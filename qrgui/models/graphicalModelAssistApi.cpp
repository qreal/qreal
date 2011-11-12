//#include "details/graphicalModel.h"
#include "graphicalModelAssistApi.h"
#include "../../qrkernel/exception/exception.h"
#include <QtCore/QUuid>
#include <QtCore/QDebug>

using namespace qReal;
using namespace models;
using namespace models::details;

GraphicalModelAssistApi::GraphicalModelAssistApi(GraphicalModel &graphicalModel, EditorManager const &editorManager)
		: mGraphicalModel(graphicalModel), mModelsAssistApi(graphicalModel, editorManager)
{
	connect(&graphicalModel, SIGNAL(nameChanged(Id)), this, SIGNAL(nameChanged(Id)));
}

EditorManager const &GraphicalModelAssistApi::editorManager() const
{
	return mModelsAssistApi.editorManager();
}

qrRepo::GraphicalRepoApi const &GraphicalModelAssistApi::graphicalRepoApi() const
{
	return mGraphicalModel.api();
}

qrRepo::GraphicalRepoApi &GraphicalModelAssistApi::mutableGraphicalRepoApi() const
{
	return mGraphicalModel.mutableApi();
}

Id GraphicalModelAssistApi::logicalId(Id const &elem) const
{
	return mGraphicalModel.data(indexById(elem), roles::logicalIdRole).value<Id>();
}

IdList GraphicalModelAssistApi::graphicalIdsByLogicalId(Id const &logicalId) const
{
	IdList result;
	QList<QPersistentModelIndex> indexes = mGraphicalModel.indexesWithLogicalId(logicalId);
	foreach (QPersistentModelIndex index, indexes) {
		result.append(idByIndex(index));
	}

	return result;
}

Id GraphicalModelAssistApi::createElement(Id const &parent, Id const &type)
{
	Q_ASSERT(type.idSize() == 3);
	Q_ASSERT(parent.idSize() == 4);

	Id const newElementId(type, QUuid::createUuid().toString());
	QString const elementFriendlyName = mModelsAssistApi.editorManager().friendlyName(type);
	mGraphicalModel.addElementToModel(parent, newElementId, Id::rootId(), "(" + elementFriendlyName + ")", QPointF(0, 0));
	return newElementId;
}

Id GraphicalModelAssistApi::createElement(Id const &parent, Id const &id, bool isFromLogicalModel, QString const &name, QPointF const &position)
{
	return mModelsAssistApi.createElement(parent, id, isFromLogicalModel, name, position);
}

Id GraphicalModelAssistApi::copyElement(Id const &source)
{
	return mGraphicalModel.mutableApi().copy(source);
}

IdList GraphicalModelAssistApi::children(Id const &element) const
{
	return mGraphicalModel.api().children(element);
}

void GraphicalModelAssistApi::changeParent(Id const &element, Id const &parent, QPointF const &position)
{
	mGraphicalModel.changeParent(mGraphicalModel.indexById(element), mGraphicalModel.indexById(parent), position);
}

void GraphicalModelAssistApi::copyProperties(Id const &dest, Id const &src)
{
	mGraphicalModel.mutableApi().copyProperties(dest, src);
}

void GraphicalModelAssistApi::stackBefore(const Id &element, const Id &sibling)
{
	mModelsAssistApi.stackBefore(element, sibling);
}

void GraphicalModelAssistApi::setConfiguration(Id const &elem, QPolygon const &newValue)
{
	mModelsAssistApi.setProperty(elem, QVariant(newValue), roles::configurationRole);
}

QPolygon GraphicalModelAssistApi::configuration(Id const &elem) const
{
	return mModelsAssistApi.property(elem, roles::configurationRole).value<QPolygon>();
}

void GraphicalModelAssistApi::setPosition(Id const &elem, QPointF const &newValue)
{
	mModelsAssistApi.setProperty(elem, QVariant(newValue), roles::positionRole);
}

QPointF GraphicalModelAssistApi::position(Id const &elem) const
{
	return mModelsAssistApi.property(elem, roles::positionRole).value<QPointF>();
}

void GraphicalModelAssistApi::setToPort(Id const &elem, qreal const &newValue)
{
	mModelsAssistApi.setProperty(elem, QVariant(newValue), roles::toPortRole);
}

qreal GraphicalModelAssistApi::toPort(Id const &elem) const
{
	return mModelsAssistApi.property(elem, roles::toPortRole).value<qreal>();
}

void GraphicalModelAssistApi::setFromPort(Id const &elem, qreal const &newValue)
{
	mModelsAssistApi.setProperty(elem, QVariant(newValue), roles::fromPortRole);
}

qreal GraphicalModelAssistApi::fromPort(Id const &elem) const
{
	return mModelsAssistApi.property(elem, roles::fromPortRole).value<qreal>();
}

void GraphicalModelAssistApi::setName(Id const &elem, QString const &newValue)
{
	mModelsAssistApi.setProperty(elem, QVariant(newValue), Qt::DisplayRole);
}

QString GraphicalModelAssistApi::name(Id const &elem) const
{
	return mModelsAssistApi.property(elem, Qt::DisplayRole).value<QString>();
}

void GraphicalModelAssistApi::setToolTip(Id const &elem, QString const &newValue)
{
	mModelsAssistApi.setProperty(elem, QVariant(newValue), Qt::ToolTipRole);
}

QString GraphicalModelAssistApi::toolTip(Id const &elem) const
{
	return mModelsAssistApi.property(elem, Qt::ToolTipRole).value<QString>();
}

bool GraphicalModelAssistApi::isGraphicalId(Id const &id) const
{
	return indexById(id) != QModelIndex();
}

IdList GraphicalModelAssistApi::temporaryRemovedLinksFrom(Id const &elem) const
{
	return graphicalRepoApi().temporaryRemovedLinksAt(elem, "from");
}

IdList GraphicalModelAssistApi::temporaryRemovedLinksTo(Id const &elem) const
{
	return graphicalRepoApi().temporaryRemovedLinksAt(elem, "to");
}

IdList GraphicalModelAssistApi::temporaryRemovedLinksNone(Id const &elem) const
{
	return graphicalRepoApi().temporaryRemovedLinksAt(elem, QString());
}

void GraphicalModelAssistApi::removeTemporaryRemovedLinks(Id const &elem)
{
	mutableGraphicalRepoApi().removeTemporaryRemovedLinks(elem);
}

void GraphicalModelAssistApi::setTo(Id const &elem, Id const &newValue)
{
	mModelsAssistApi.setTo(elem, newValue);
}

Id GraphicalModelAssistApi::to(Id const &elem) const
{
	return mModelsAssistApi.to(elem);
}

void GraphicalModelAssistApi::setFrom(Id const &elem, Id const &newValue)
{
	mModelsAssistApi.setFrom(elem, newValue);
}

Id GraphicalModelAssistApi::from(Id const &elem) const
{
	return mModelsAssistApi.from(elem);
}

QModelIndex GraphicalModelAssistApi::indexById(Id const &id) const
{
	return mModelsAssistApi.indexById(id);
}

Id GraphicalModelAssistApi::idByIndex(QModelIndex const &index) const
{
	return mModelsAssistApi.idByIndex(index);
}

QPersistentModelIndex GraphicalModelAssistApi::rootIndex() const
{
	return mModelsAssistApi.rootIndex();
}

Id GraphicalModelAssistApi::rootId() const
{
	return mModelsAssistApi.rootId();
}

bool GraphicalModelAssistApi::hasRootDiagrams() const
{
	return mModelsAssistApi.hasRootDiagrams();
}

int GraphicalModelAssistApi::childrenOfRootDiagram() const
{
	return mModelsAssistApi.childrenOfRootDiagram();
}

int GraphicalModelAssistApi::childrenOfDiagram(const Id &parent) const
{
	return mModelsAssistApi.childrenOfDiagram(parent);
}
