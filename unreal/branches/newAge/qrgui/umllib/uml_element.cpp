#include "uml_element.h"

#include <QtGui>

using namespace qReal;
using namespace UML;

Element::Element()
	: mMoving(false)
	, mElementImpl(NULL)
	, mGraphicalAssistApi(NULL)
	, mLogicalAssistApi(NULL)
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemClipsChildrenToShape |
		ItemClipsToShape | ItemSendsGeometryChanges);
	setAcceptDrops(true);
	setCursor(Qt::PointingHandCursor);
}

void Element::setId(qReal::Id &id)
{
	mId = id;
	update();
}

Id Element::id() const
{
	return mId;
}

QPersistentModelIndex Element::index() const
{
	return mGraphicalAssistApi->indexById(id());
}

void Element::updateData()
{
	setToolTip(mGraphicalAssistApi->toolTip(id()));
}

QList<ContextMenuAction*> Element::contextMenuActions()
{
	return QList<ContextMenuAction*>();
}

QString Element::property(QString const &roleName) const
{
	return mLogicalAssistApi->propertyByRoleName(id(), roleName).toString();
}

void Element::setProperty(QString const &roleName, QString const &value)
{
	mLogicalAssistApi->setPropertyByRoleName(id(), value, roleName);
}

void Element::setAssistApi(qReal::models::GraphicalModelAssistApi *graphicalAssistApi, qReal::models::LogicalModelAssistApi *logicalAssistApi)
{
	mGraphicalAssistApi = graphicalAssistApi;
	mLogicalAssistApi = logicalAssistApi;
}
