#pragma once

#include <QtWidgets/QGraphicsLinearLayout>

#include "layoutHandler.h"
#include "../../utilsDeclSpec.h"

namespace qReal
{
namespace layouts
{

/// Layout manager wrapper for linear layout
class QRUTILS_EXPORT LinearLayoutHandler : public LayoutHandler
{
	Q_OBJECT

public:
	LinearLayoutHandler(Qt::Orientation orientation, QGraphicsWidget *layoutHost);
	virtual ~LinearLayoutHandler() {}

protected:
	virtual void addItemTo(QGraphicsLayoutItem *item, QPointF const &pos);
	virtual void removeItem(QGraphicsLayoutItem *item);

	virtual QGraphicsLayout *generateLayout();
	virtual void placeChildrenWithoutLayout();

private:
	int cellToAdd(QPointF const &position) const;
	int actualDimension(QPointF const &point) const;
	int indexOf(QGraphicsLayoutItem * const item) const;

	Qt::Orientation mOrientation;
	QGraphicsLinearLayout *mLayout;
};

/// A functor comparator for sorting in top-left corner
/// ascending order
class LinearLayoutItemComparator
{
public:
	explicit LinearLayoutItemComparator(Qt::Orientation orientation);

	bool operator()(const QGraphicsLayoutItem *left
			, const QGraphicsLayoutItem *right) const;

private:
	int actualDimension(QPointF const &point) const;

	Qt::Orientation mOrientation;
};

}
}
