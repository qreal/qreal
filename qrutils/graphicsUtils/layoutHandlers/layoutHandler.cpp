#include <QtGui/QWidget>
#include <QtGui/QPalette>
#include <QtGui/QGraphicsProxyWidget>
#include <QtGui/QGraphicsDropShadowEffect>

#include "layoutHandler.h"

using namespace qReal::layouts;

LayoutHandler::LayoutHandler(QGraphicsWidget *layoutHost)
	: QObject(layoutHost)
	, mItem(layoutHost)
	, mEnabled(false)
{
	initPlaceholder();
}

void LayoutHandler::initPlaceholder()
{
	QWidget *placeholderWidget = new QWidget;
	QPalette placeholderPalette = placeholderWidget->palette();
	QColor const color(0, 150, 200, 50);
	placeholderPalette.setColor(QPalette::Window, color);
	placeholderWidget->setPalette(placeholderPalette);
	// Without it Qt graphics layout manager shrinks geometry to 0
	placeholderWidget->setMinimumSize(1, 1);

	mPlaceholder = new QGraphicsProxyWidget;
	QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
	effect->setBlurRadius(5);
	effect->setOffset(0);
	effect->setColor(Qt::black);
	mPlaceholder->setGraphicsEffect(effect);
	mPlaceholder->setWidget(placeholderWidget);
}

void LayoutHandler::handleDragEnter(QGraphicsLayoutItem const *draggedItem
		, QPointF const &position)
{
	if (!mEnabled) {
		return;
	}
	drawPlaceholder(position);
}

void LayoutHandler::handleDragMove(QGraphicsLayoutItem const *draggedItem
		, QPointF const &position)
{
	if (!mEnabled) {
		return;
	}
	drawPlaceholder(position);
}

void LayoutHandler::handleDragLeave()
{
	if (!mEnabled) {
		return;
	}
	erasePlaceholder();
}

void LayoutHandler::handleDropEvent(QGraphicsLayoutItem *draggedItem
		, QPointF const &position)
{
	if (!mEnabled) {
		return;
	}
	addItemTo(draggedItem, position);
	erasePlaceholder();
}

void LayoutHandler::setEnabled(bool enabled)
{
	mEnabled = enabled;
	if (enabled && (mItem->layout() != mLayout || !mLayout)) {
		mLayout = generateLayout();
		mItem->setLayout(mLayout);
		placeChildrenWithoutLayout();
	} else {
		nullifyLayout();
	}
}

void LayoutHandler::nullifyLayout()
{
	if (mItem->layout()) {
		mItem->setLayout(NULL);
	}
}

void LayoutHandler::drawPlaceholder(QPointF const &position)
{
	addItemTo(mPlaceholder, position);
	mPlaceholder->setVisible(true);
}

void LayoutHandler::erasePlaceholder()
{
	removeItem(mPlaceholder);
	mPlaceholder->setParentItem(0);
	mPlaceholder->setVisible(false);
}

QList<QGraphicsLayoutItem *> LayoutHandler::childrenWithoutLayout() const
{
	QList<QGraphicsLayoutItem *> result;
	foreach (QGraphicsItem *item, mItem->childItems()) {
		QGraphicsLayoutItem *layoutItem = dynamic_cast<QGraphicsLayoutItem *>(item);
		bool const inLayout = layoutItem->parentLayoutItem()
				&& layoutItem->parentLayoutItem()->isLayout();
		if (layoutItem && !inLayout) {
			result << layoutItem;
		}
	}
	return result;
}
