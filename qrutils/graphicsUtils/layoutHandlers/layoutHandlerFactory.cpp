#include "layoutHandlerFactory.h"

using namespace qReal::layouts;

LayoutHandlerFactory::LayoutHandlerFactory(QGraphicsWidget *layoutHost)
	: QObject(layoutHost)
	, mCurrentType(none)
	, mVerticalHandler(new LinearLayoutHandler(Qt::Vertical, layoutHost))
	, mHorizontalHandler(new LinearLayoutHandler(Qt::Horizontal, layoutHost))
	, mGridHandler(NULL)
{
}

LayoutHandler *LayoutHandlerFactory::handler(LayoutType type) const
{
	switch (type) {
	case vertical:
		return mVerticalHandler;
	case horizontal:
		return mHorizontalHandler;
	case grid:
		return mGridHandler;
	case none:
	default:
		return NULL;
	}
}

LayoutType LayoutHandlerFactory::currentType() const
{
	return mCurrentType;
}

LayoutHandler *LayoutHandlerFactory::currentHandler() const
{
	return handler(mCurrentType);
}

bool LayoutHandlerFactory::hasLayout() const
{
	return currentType() == none;
}

void LayoutHandlerFactory::setType(LayoutType type)
{
	if (mCurrentType == type) {
		return;
	}

	enableCurrentHandler(false);
	mCurrentType = type;
	enableCurrentHandler(true);
}

void LayoutHandlerFactory::enableCurrentHandler(bool enable)
{
	LayoutHandler *curHandler = currentHandler();
	if (curHandler) {
		curHandler->setEnabled(enable);
	}
}

void LayoutHandlerFactory::handleDragEnter(QGraphicsLayoutItem const *draggedItem, QPointF const &position)
{
	LayoutHandler *curHandler = currentHandler();
	if (curHandler) {
		curHandler->handleDragEnter(draggedItem, position);
	}
}

void LayoutHandlerFactory::handleDragMove(QGraphicsLayoutItem const *draggedItem, QPointF const &position)
{
	LayoutHandler *curHandler = currentHandler();
	if (curHandler) {
		curHandler->handleDragMove(draggedItem, position);
	}
}

void LayoutHandlerFactory::handleDragLeave()
{
	LayoutHandler *curHandler = currentHandler();
	if (curHandler) {
		curHandler->handleDragLeave();
	}
}

void LayoutHandlerFactory::handleDropEvent(QGraphicsLayoutItem *draggedItem, QPointF const &position)
{
	LayoutHandler *curHandler = currentHandler();
	if (curHandler) {
		curHandler->handleDropEvent(draggedItem, position);
	}
}
