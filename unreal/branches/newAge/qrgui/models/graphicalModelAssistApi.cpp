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
}

qrRepo::GraphicalRepoApi const &GraphicalModelAssistApi::graphicalRepoApi() const
{
	return mGraphicalModel.api();
}

Id GraphicalModelAssistApi::createElement(qReal::Id const &parent, qReal::Id const &type)
{
	Q_ASSERT(type.idSize() == 3);
	Q_ASSERT(parent.idSize() == 4);

	Id const newElementId(type, QUuid::createUuid().toString());
	QString const elementFriendlyName = mEditorManager.friendlyName(type);
	mGraphicalModel.addElementToModel(parent, newElementId, Id::rootId(), "(" + elementFriendlyName + ")", QPointF(0, 0));
	return newElementId;
}

IdList GraphicalModelAssistApi::children(qReal::Id const &element) const
{
	return mGraphicalModel.api().children(element);
}

void GraphicalModelAssistApi::changeParent(qReal::Id const &element, qReal::Id const &parent, QPointF const &position)
{
	mGraphicalModel.changeParent(mModel.indexById(element), mModel.indexById(parent), position);
}

void GraphicalModelAssistApi::setConfiguration(qReal::Id const &elem, QPolygon const &newValue)
{
	setProperty(elem, QVariant(newValue), roles::configurationRole);
}

QPolygon GraphicalModelAssistApi::configuration(qReal::Id const &elem) const
{
	return property(elem, roles::configurationRole).value<QPolygon>();
}

void GraphicalModelAssistApi::setPosition(qReal::Id const &elem, QPointF const &newValue)
{
	setProperty(elem, QVariant(newValue), roles::positionRole);
}

QPointF GraphicalModelAssistApi::position(qReal::Id const &elem) const
{
	return property(elem, roles::positionRole).value<QPointF>();
}

void GraphicalModelAssistApi::setToPort(qReal::Id const &elem, qreal const &newValue)
{
	setProperty(elem, QVariant(newValue), roles::toPortRole);
}

qreal GraphicalModelAssistApi::toPort(qReal::Id const &elem) const
{
	return property(elem, roles::toPortRole).value<qreal>();
}

void GraphicalModelAssistApi::setFromPort(qReal::Id const &elem, qreal const &newValue)
{
	setProperty(elem, QVariant(newValue), roles::fromPortRole);
}

qreal GraphicalModelAssistApi::fromPort(qReal::Id const &elem) const
{
	return property(elem, roles::fromPortRole).value<qreal>();
}

void GraphicalModelAssistApi::setName(qReal::Id const &elem, QString const &newValue)
{
	setProperty(elem, QVariant(newValue), Qt::DisplayRole);
}

QString GraphicalModelAssistApi::name(qReal::Id const &elem) const
{
	return property(elem, Qt::DisplayRole).value<QString>();
}
