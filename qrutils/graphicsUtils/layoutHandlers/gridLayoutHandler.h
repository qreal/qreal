#pragma once

#include <QtWidgets/QGraphicsGridLayout>

#include "layoutHandler.h"
#include "../../utilsDeclSpec.h"

namespace qReal
{
namespace layouts
{

/// Layout manager wrapper for grid layout
class QRUTILS_EXPORT GridLayoutHandler : public LayoutHandler
{
	Q_OBJECT

public:
	explicit GridLayoutHandler(QGraphicsWidget *layoutHost);
	virtual ~GridLayoutHandler() {}

protected:
	virtual void addItemTo(QGraphicsLayoutItem *item, QPointF const &pos);
	virtual void removeItem(QGraphicsLayoutItem *item);

	virtual QGraphicsLayout *generateLayout();
	virtual void placeChildrenWithoutLayout();

private:
	static qreal constexpr veryLargeNumber = 1e10;

	int rowToAdd(QPointF const &position) const;

	bool isCellFree(int row, int column) const;
	bool isRowEmpty(int row) const;
	bool isColumnEmpty(int row) const;
	bool containsItem(QGraphicsLayoutItem const *item) const;

	bool processShiftCase(QGraphicsLayoutItem *item, QPointF const &position
		, int &firstOccupiedRow, int &firstOccupiedColumn);
	void minimizeDistance(QRectF const &cell, QPointF const &position
			, qreal &currentMinimum, int &closestRow, int &closestColumn
			, int currentRow, int currentColumn);

	void rebuildGridMap();
	void rebuildRowsMap();
	void rebuildColumnsMap();

	void insertItem(QGraphicsLayoutItem *item, int row, int column);
	int findPosition(qreal const point
			, QVector<qreal> const &map
			, int searchStart) const;

	void shiftItems(int rowShift, int columnShift);

	QGraphicsGridLayout *mLayout;
	/// Contains info about grid rows upper border coordiates
	QVector<qreal> mRowMap;
	/// Contains info about grid columns left border coordiates
	QVector<qreal> mColumnMap;
};

}
}
