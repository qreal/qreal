#pragma once

#include <QtGui/QGraphicsLinearLayout>
#include <QtGui/QGraphicsProxyWidget>

#include "../../utilsDeclSpec.h"

namespace qReal
{
namespace layouts
{

QSizeF const DEFAULT_PLACEHOLDER_SIZE = QSizeF(50, 50);
int const DEFAULT_LAYOUT_MARGIN = 5;

/// Base class for all NodeElement layout managers
class QRUTILS_EXPORT LayoutHandler : public QObject
{
	Q_OBJECT

public:
	virtual ~LayoutHandler() {}

	void handleDragEnter(QGraphicsLayoutItem const *draggedItem, QPointF const &position);
	void handleDragMove(QGraphicsLayoutItem const *draggedItem, QPointF const &position);
	void handleDragLeave();
	void handleDropEvent(QGraphicsLayoutItem *draggedItem, QPointF const &position);

	virtual void setEnabled(bool enabled);

	void setOuterMargin(int left, int top, int right, int bottom);
	void setLayoutMargin(int left, int top, int right, int bottom);

protected:
	LayoutHandler(QGraphicsWidget *layoutHost /* Takes ownership on this */);

	QGraphicsLayoutItem const *placeholder() const;
	void nullifyLayout();

	/// Implementation must place @param item into target layout into target position;
	/// if @param item is already placed but position in layout differs
	/// from the old one then @param item must be moved to new position
	virtual void addItemTo(QGraphicsLayoutItem *item, QPointF const &pos) = 0;
	virtual void removeItem(QGraphicsLayoutItem *item) = 0;

	/// Implementation must initialize a new graphics layout instance
	/// and return it
	virtual QGraphicsLayout *generateLayout() = 0;

	/// Implementation must place all of our element`s child items
	/// without layout of into current layout
	virtual void placeChildrenWithoutLayout() = 0;

	QList<QGraphicsLayoutItem *> childrenWithoutLayout() const;

	QGraphicsWidget *mItem;

private slots:
	/// Tells Qt layout manager about current item size for correct
	/// items alignmet. Nobody knows why Qt managers don`t sync sizes itself
	void synchronizeSizes();

	/// Tells our layout host not to shrink when layout minimum reached
	void synchronizeMinimalSizes();

private:
	void initPlaceholder();
	void drawPlaceholder(QPointF const &position, QGraphicsLayoutItem const *prototype);
	void erasePlaceholder();

	void reinitLayouts();

	bool mEnabled;
	QGraphicsProxyWidget *mPlaceholder;
	QGraphicsLayout *mLayout;
	QGraphicsLinearLayout *mLayoutWrapper;
	int mWrapperLeftMargin;
	int mWrapperTopMargin;
	int mWrapperRightMargin;
	int mWrapperBottomMargin;
	int mLeftMargin;
	int mTopMargin;
	int mRightMargin;
	int mBottomMargin;
};

}
}
