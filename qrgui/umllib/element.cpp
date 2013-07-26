#include <QtWidgets>

#include "element.h"
#include "../controller/commands/changePropertyCommand.h"

using namespace qReal;

Element::Element(ElementImpl* elementImpl)
	: mMoving(false)
	, mElementImpl(elementImpl)
	, mLogicalAssistApi(NULL)
	, mGraphicalAssistApi(NULL)
	, mController(NULL)
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

QList<ContextMenuAction*> Element::contextMenuActions(const QPointF &pos)
{
	Q_UNUSED(pos)
	return QList<ContextMenuAction*>();
}

QString Element::logicalProperty(QString const &roleName) const
{
	return mLogicalAssistApi->propertyByRoleName(logicalId(), roleName).toString();
}

void Element::setLogicalProperty(QString const &roleName, QString const &value)
{
	mController->execute(new commands::ChangePropertyCommand(mLogicalAssistApi, roleName, logicalId(), value));
}

void Element::setAssistApi(qReal::models::GraphicalModelAssistApi *graphicalAssistApi, qReal::models::LogicalModelAssistApi *logicalAssistApi)
{
	mGraphicalAssistApi = graphicalAssistApi;
	mLogicalAssistApi = logicalAssistApi;
}

void Element::setController(Controller *controller)
{
	mController = controller;
}

void Element::initTitles()
{
}

void Element::singleSelectionState(const bool singleSelected)
{
	if (singleSelected) {
		selectionState(true);
	}
	emit switchFolding(!singleSelected);
}

void Element::selectionState(const bool selected)
{
	if (isSelected() != selected) {
		setSelected(selected);
	}
	if (!selected) {
		singleSelectionState(false);
	}

	foreach (Label *title, mTitles) {
		title->setParentSelected(selected);
	}
}

ElementImpl* Element::elementImpl() const
{
	return mElementImpl;
}

void Element::setTitlesVisible(bool visible)
{
	mTitlesVisible = visible;
	setTitlesVisiblePrivate(visible);
}

void Element::setTitlesVisiblePrivate(bool visible)
{
	foreach (Label * const title, mTitles) {
		title->setVisible(title->isHard() || visible);
	}
}
