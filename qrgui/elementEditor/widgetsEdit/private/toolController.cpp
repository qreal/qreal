#include "toolController.h"
#include <qrgui/elementEditor/widgetsEdit/tools/tool.h>
#include <qrgui/elementEditor/widgetsEdit/tools/layoutTool.h>
#include <qrgui/elementEditor/widgetsEdit/tools/layoutHelpers/layoutHelperFactory.h>

using namespace qReal::widgetsEdit;

ToolController::ToolController()
	: mDraggedChild(NULL)
{
}

Tool *ToolController::selectedItem() const
{
	return mSelectedChild;
}

void ToolController::addChild(Tool *child)
{
	mItems << child;
	select(child);
}

void ToolController::removeChild(Tool *child)
{
	if (!child) {
		return;
	}

	foreach (QGraphicsItem *item, child->childItems()) {
		Tool *tool = dynamic_cast<Tool *>(item);
		if (tool) {
			removeChild(tool);
		} else {
			child->childItems().removeOne(tool);
			delete tool;
		}
	}

	Tool *parent = dynamic_cast<Tool *>(child->parentItem());
	if (!parent) {
		return;
	}
	mItems.removeOne(child);
	parent->removeChild(child);
	delete child;
	select(mItems[0]);
}

void ToolController::select(Tool *child)
{
	if (child->selected()) {
		return;
	}
	mSelectedChild = child;
	unselectAll();
	mSelectedChild->select();
	if (mBrowserController) {
		mBrowserController->setController(mSelectedChild->propertyManager());
	}
	emit selectionChanged(mSelectedChild);
}

void ToolController::markChildDragged(Tool *child)
{
	mDraggedChild = child;
	mDraggedChildOldParent = dynamic_cast<LayoutTool *>(child->parentItem());
	if (!mDraggedChildOldParent) {
		return;
	}

	mDraggedChildOldParent->startChildDrag(mDraggedChild);
	child->hide();
}

Tool *ToolController::draggedChild() const
{
	return mDraggedChild;
}

void ToolController::unmarkChildDragged(bool success)
{
	if (!mDraggedChild || !mDraggedChildOldParent) {
		return;
	}

	mDraggedChildOldParent->finishChildDrag(success);

	mDraggedChild->show();
	mDraggedChild = NULL;
	mDraggedChildOldParent = NULL;
}

void ToolController::setLayout(Tool *tool, LayoutType type)
{
	LayoutTool *layoutTool = dynamic_cast<LayoutTool *>(tool);
	if (layoutTool) {
		layoutTool->layoutFactory()->setLayout(type);
	}
}

void ToolController::move(int dx, int dy)
{
	Tool *item = selectedItem();
	if (!item || !item->movable()) {
		return;
	}
	Tool *parentItem = dynamic_cast<Tool *>(item->parentItem());
	if (!parentItem || parentItem->layout()) {
		return;
	}
	item->moveBy(dx, dy);
}

void ToolController::setPropertyBrowserController(PropertyBrowserController *controller)
{
	mBrowserController = controller;
}

void ToolController::unselectAll()
{
	foreach (Tool *child, mItems) {
		child->unselect();
	}
}

void ToolController::processKeyEvent(QKeyEvent *event)
{
	if (event->matches(QKeySequence::Delete)) {
		removeChild(selectedItem());

		event->accept();
		return;
	}

	if (event->key() == Qt::Key_Right || event->key() == Qt::Key_Left ||
			event->key() == Qt::Key_Up || event->key() == Qt::Key_Down) {
		int const delta = event->modifiers() == Qt::ControlModifier ? 1 : 5;
		int dx = 0;
		int dy = 0;
		switch(event->key()) {
		case Qt::Key_Right:
			dx = delta;
			break;
		case Qt::Key_Left:
			dx = -delta;
			break;
		case Qt::Key_Up:
			dy = -delta;
			break;
		case Qt::Key_Down:
			dy = delta;
			break;
		}
		move(dx, dy);

		event->accept();
		return;
	}

	if (event->matches(QKeySequence::Save)) {
		emit savePressed();
	} else if (event->matches(QKeySequence::SaveAs)) {
		emit saveAsPressed();
	} else if (event->matches(QKeySequence::Open)) {
		emit openPressed();
	}
}
