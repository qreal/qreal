#pragma once

#include <QtGui/QGraphicsProxyWidget>

#include "../../utilsDeclSpec.h"

namespace qReal
{
namespace layouts
{

QSizeF const DEFAULT_PLACEHOLDER_SIZE = QSizeF(50, 50);

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

protected:
	LayoutHandler(QGraphicsWidget *layoutHost /* Takes ownership on this */);

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

private:
	void initPlaceholder();
	void drawPlaceholder(QPointF const &position);
	void erasePlaceholder();

	bool mEnabled;
	QGraphicsProxyWidget *mPlaceholder;
	QGraphicsLayout *mLayout;
};

}
}
