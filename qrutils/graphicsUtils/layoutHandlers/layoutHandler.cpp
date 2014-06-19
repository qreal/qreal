#include <QtGui/QPalette>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsProxyWidget>
#include <QtWidgets/QGraphicsDropShadowEffect>

#include "layoutHandler.h"

using namespace qReal::layouts;

LayoutHandler::LayoutHandler(QGraphicsWidget *layoutHost)
	: QObject(layoutHost)
	, mItem(layoutHost)
	, mEnabled(false)
	, mLayoutWrapper(NULL)
	, mWrapperLeftMargin(DEFAULT_LAYOUT_MARGIN)
	, mWrapperTopMargin(DEFAULT_LAYOUT_MARGIN)
	, mWrapperRightMargin(DEFAULT_LAYOUT_MARGIN)
	, mWrapperBottomMargin(DEFAULT_LAYOUT_MARGIN)
	, mLeftMargin(DEFAULT_LAYOUT_MARGIN)
	, mTopMargin(DEFAULT_LAYOUT_MARGIN)
	, mRightMargin(DEFAULT_LAYOUT_MARGIN)
	, mBottomMargin(DEFAULT_LAYOUT_MARGIN)
{
	initPlaceholder();
}

QGraphicsLayoutItem const *LayoutHandler::placeholder() const
{
	return mPlaceholder;
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
	drawPlaceholder(position, draggedItem);
}

void LayoutHandler::handleDragMove(QGraphicsLayoutItem const *draggedItem
		, QPointF const &position)
{
	if (!mEnabled) {
		return;
	}
	drawPlaceholder(position, draggedItem);
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
	erasePlaceholder();
	addItemTo(draggedItem, position);
	synchronizeMinimalSizes();
}

void LayoutHandler::setEnabled(bool enabled)
{
	mEnabled = enabled;
	if (enabled && (mItem->layout() != mLayout || !mLayout)) {
		reinitLayouts();
	} else {
		nullifyLayout();
	}
}

void LayoutHandler::setOuterMargin(int left, int top, int right, int bottom)
{
	mWrapperLeftMargin = left;
	mWrapperTopMargin = top;
	mWrapperRightMargin = right;
	mWrapperBottomMargin = bottom;
}

void LayoutHandler::setLayoutMargin(int left, int top, int right, int bottom)
{
	mLeftMargin = left;
	mTopMargin = top;
	mRightMargin = right;
	mBottomMargin = bottom;
}

void LayoutHandler::nullifyLayout()
{
	if (mItem->layout()) {
		mItem->setLayout(NULL);
	}
	mLayout = NULL;
	mLayoutWrapper = NULL;
	disconnect(this, SLOT(synchronizeSizes()));
}

void LayoutHandler::drawPlaceholder(QPointF const &position
		, QGraphicsLayoutItem const *prototype)
{
	if (prototype) {
		mPlaceholder->setSizePolicy(prototype->sizePolicy().horizontalPolicy()
			, prototype->sizePolicy().verticalPolicy());
		mPlaceholder->setPreferredSize(prototype->preferredSize());
	} else {
		mPlaceholder->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
		mPlaceholder->setPreferredSize(DEFAULT_PLACEHOLDER_SIZE);
	}
	addItemTo(mPlaceholder, position);
	mPlaceholder->setVisible(true);
}

void LayoutHandler::erasePlaceholder()
{
	removeItem(mPlaceholder);
	mPlaceholder->setParentItem(0);
	mPlaceholder->setVisible(false);
}

void LayoutHandler::reinitLayouts()
{
	mLayoutWrapper = new QGraphicsLinearLayout(Qt::Vertical);
	mLayoutWrapper->setContentsMargins(mWrapperLeftMargin, mWrapperTopMargin
			, mWrapperRightMargin, mWrapperBottomMargin);
	mItem->setLayout(mLayoutWrapper);
	mLayout = generateLayout();
	mLayout->setContentsMargins(mLeftMargin, mTopMargin, mRightMargin, mBottomMargin);
	mLayoutWrapper->addItem(mLayout);
	placeChildrenWithoutLayout();
	synchronizeSizes();
	connect(mItem, SIGNAL(geometryChanged()), this, SLOT(synchronizeSizes()));
}

QList<QGraphicsLayoutItem *> LayoutHandler::childrenWithoutLayout() const
{
	QList<QGraphicsLayoutItem *> result;
	foreach (QGraphicsItem *item, mItem->childItems()) {
		QGraphicsLayoutItem *layoutItem = dynamic_cast<QGraphicsLayoutItem *>(item);
		bool const inLayout = layoutItem && layoutItem->parentLayoutItem()
				&& layoutItem->parentLayoutItem()->isLayout();
		if (layoutItem && !inLayout) {
			result << layoutItem;
		}
	}
	return result;
}

void LayoutHandler::synchronizeSizes()
{
	QSizeF const size = mItem->geometry().size();
	QSizeF const innerSize = QSizeF
				( size.width() - mWrapperLeftMargin - mWrapperRightMargin
				, size.height() - mWrapperTopMargin - mWrapperBottomMargin);
	if (mLayout && mLayoutWrapper) {
		mLayoutWrapper->setPreferredSize(size);
		mLayout->setPreferredSize(innerSize);
		synchronizeMinimalSizes();
	}
}

void LayoutHandler::synchronizeMinimalSizes()
{
	QSizeF const innerSize = mLayout->minimumSize();
	QSizeF const outerSize = QSizeF
			( innerSize.width() + mWrapperLeftMargin + mWrapperRightMargin
			, innerSize.height() + mWrapperTopMargin + mWrapperBottomMargin);
	mItem->setMinimumSize(outerSize);
}
