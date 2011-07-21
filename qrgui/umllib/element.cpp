#include "element.h"

#include <QtGui>

using namespace qReal;

Element::Element()
	: mMoving(false)
	, mElementImpl(NULL)
	, mLogicalAssistApi(NULL)
	, mGraphicalAssistApi(NULL)
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

qReal::Id Element::logicalId() const
{
	return mGraphicalAssistApi->logicalId(mId);
}

QString Element::name() const
{
	return mGraphicalAssistApi->name(id());
}

void Element::updateData()
{
	setToolTip(mGraphicalAssistApi->toolTip(id()));
}

QList<ContextMenuAction*> Element::contextMenuActions()
{
	return QList<ContextMenuAction*>();
}

QString Element::logicalProperty(QString const &roleName) const
{
	return mLogicalAssistApi->propertyByRoleName(logicalId(), roleName).toString();
}

void Element::setLogicalProperty(QString const &roleName, QString const &value)
{
	mLogicalAssistApi->setPropertyByRoleName(logicalId(), value, roleName);
}

void Element::setAssistApi(qReal::models::GraphicalModelAssistApi *graphicalAssistApi, qReal::models::LogicalModelAssistApi *logicalAssistApi)
{
	mGraphicalAssistApi = graphicalAssistApi;
	mLogicalAssistApi = logicalAssistApi;
}

void Element::initTitlesBy(QRectF const& contents)
{
	foreach (ElementTitle * const title, mTitles) {
		title->transform(contents);
	}
}

void Element::initTitles()
{
	initTitlesBy(boundingRect().adjusted(kvadratik, kvadratik, -kvadratik, -kvadratik));
}

void Element::singleSelectionState(const bool singleSelected) {
	emit switchFolding(!singleSelected);
}
void Element::selectionState(const bool selected) {
	//it will be usefull in the future
}
