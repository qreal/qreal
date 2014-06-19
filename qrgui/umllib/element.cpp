#include "element.h"

#include "controller/commands/changePropertyCommand.h"

using namespace qReal;

Element::Element(ElementImpl *elementImpl
		, Id const &id
		, qReal::models::GraphicalModelAssistApi &graphicalAssistApi
		, qReal::models::LogicalModelAssistApi &logicalAssistApi
		)
	: QGraphicsProxyWidget()
	, mMoving(false)
	, mId(id)
	, mElementImpl(elementImpl)
	, mLogicalAssistApi(logicalAssistApi)
	, mGraphicalAssistApi(graphicalAssistApi)
	, mController(NULL)
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemClipsChildrenToShape |
			ItemClipsToShape | ItemSendsGeometryChanges);

	setAcceptDrops(true);
	setCursor(Qt::PointingHandCursor);
}

Id Element::id() const
{
	return mId;
}

qReal::Id Element::logicalId() const
{
	return mGraphicalAssistApi.logicalId(mId);
}

QString Element::name() const
{
	return mGraphicalAssistApi.name(id());
}

void Element::updateData()
{
	setToolTip(mGraphicalAssistApi.toolTip(id()));
}

QList<ContextMenuAction*> Element::contextMenuActions(const QPointF &pos)
{
	Q_UNUSED(pos)
	return QList<ContextMenuAction*>();
}

QVariant Element::logicalProperty(QString const &roleName) const
{
	return mLogicalAssistApi.propertyByRoleName(logicalId(), roleName);
}

void Element::setLogicalProperty(QString const &roleName, QString const &value, bool withUndoRedo)
{
	commands::AbstractCommand *command = new commands::ChangePropertyCommand(&mLogicalAssistApi
			, roleName, logicalId(), value);
	if (withUndoRedo) {
		mController->execute(command);
	} else {
		command->redo();
		delete command;
	}
}

void Element::setController(Controller *controller)
{
	mController = controller;
}

qReal::Controller * Element::controller() const
{
	return mController;
}

void Element::initTitles()
{
}

void Element::select(const bool singleSelected)
{
	if (singleSelected) {
		setSelectionState(true);
	}
	emit switchFolding(!singleSelected);
}

void Element::setSelectionState(const bool selected)
{
	if (isSelected() != selected) {
		setSelected(selected);
	}
	if (!selected) {
		select(false);
	}

	foreach (Label * const label, mLabels) {
		label->setParentSelected(selected);
	}
}

ElementImpl* Element::elementImpl() const
{
	return mElementImpl;
}

void Element::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsProxyWidget::mousePressEvent(event);
	event->accept();
	QGraphicsItem::mousePressEvent(event);
}

void Element::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsProxyWidget::mouseMoveEvent(event);
	QGraphicsItem::mouseMoveEvent(event);
}

void Element::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsProxyWidget::mouseReleaseEvent(event);
	QGraphicsItem::mouseReleaseEvent(event);
}

bool Element::createChildrenFromMenu() const
{
	return mElementImpl->createChildrenFromMenu();
}

void Element::setTitlesVisible(bool visible)
{
	mTitlesVisible = visible;
	setTitlesVisiblePrivate(visible);
}

void Element::setTitlesVisiblePrivate(bool visible)
{
	foreach (Label const * const label, mLabels) {
		if (label->isSelected()) {
			return;
		}
	}

	foreach (Label * const label, mLabels) {
		label->setVisible(label->isHard() || visible);
	}
}
