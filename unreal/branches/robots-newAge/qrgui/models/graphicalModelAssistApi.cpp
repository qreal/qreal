//#include "details/graphicalModel.h"
#include "graphicalModelAssistApi.h"
#include "../kernel/exception/exception.h"
#include <QtCore/QUuid>
#include <QtCore/QDebug>

using namespace qReal;
using namespace models;
using namespace details;

GraphicalModelAssistApi::GraphicalModelAssistApi(GraphicalModel &graphicalModel, EditorManager const &editorManager)
	: ModelsAssistApi(graphicalModel, editorManager), mGraphicalModel(graphicalModel)
{
	connect(&graphicalModel, SIGNAL(nameChanged(Id)), this, SIGNAL(nameChanged(Id)));
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
	QString const elementFriendlyName = mEditorManager.friendlyName(type);
	mGraphicalModel.addElementToModel(parent, newElementId, Id::rootId(), "(" + elementFriendlyName + ")", QPointF(0, 0));
	return newElementId;
}

Id GraphicalModelAssistApi::createElement(Id const &parent, Id const &id, bool isFromLogicalModel, QString const &name, QPointF const &position)
{
	return ModelsAssistApi::createElement(parent, id, isFromLogicalModel, name, position);
}

IdList GraphicalModelAssistApi::children(Id const &element) const
{
	return mGraphicalModel.api().children(element);
}

void GraphicalModelAssistApi::changeParent(Id const &element, Id const &parent, QPointF const &position)
{
	mGraphicalModel.changeParent(mModel.indexById(element), mModel.indexById(parent), position);
}

void GraphicalModelAssistApi::setConfiguration(Id const &elem, QPolygon const &newValue)
{
	ModelsAssistApi::setProperty(elem, QVariant(newValue), roles::configurationRole);
}

QPolygon GraphicalModelAssistApi::configuration(Id const &elem) const
{
	return ModelsAssistApi::property(elem, roles::configurationRole).value<QPolygon>();
}

void GraphicalModelAssistApi::setPosition(Id const &elem, QPointF const &newValue)
{
	ModelsAssistApi::setProperty(elem, QVariant(newValue), roles::positionRole);
}

QPointF GraphicalModelAssistApi::position(Id const &elem) const
{
	return ModelsAssistApi::property(elem, roles::positionRole).value<QPointF>();
}

void GraphicalModelAssistApi::setToPort(Id const &elem, qreal const &newValue)
{
	ModelsAssistApi::setProperty(elem, QVariant(newValue), roles::toPortRole);
}

qreal GraphicalModelAssistApi::toPort(Id const &elem) const
{
	return ModelsAssistApi::property(elem, roles::toPortRole).value<qreal>();
}

void GraphicalModelAssistApi::setFromPort(Id const &elem, qreal const &newValue)
{
	ModelsAssistApi::setProperty(elem, QVariant(newValue), roles::fromPortRole);
}

qreal GraphicalModelAssistApi::fromPort(Id const &elem) const
{
	return ModelsAssistApi::property(elem, roles::fromPortRole).value<qreal>();
}

void GraphicalModelAssistApi::setName(Id const &elem, QString const &newValue)
{
	ModelsAssistApi::setProperty(elem, QVariant(newValue), Qt::DisplayRole);
}

QString GraphicalModelAssistApi::name(Id const &elem) const
{
	return ModelsAssistApi::property(elem, Qt::DisplayRole).value<QString>();
}

void GraphicalModelAssistApi::setToolTip(Id const &elem, QString const &newValue)
{
	ModelsAssistApi::setProperty(elem, QVariant(newValue), Qt::ToolTipRole);
}

QString GraphicalModelAssistApi::toolTip(Id const &elem) const
{
	return ModelsAssistApi::property(elem, Qt::ToolTipRole).value<QString>();
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
