#include "gridLayoutHandler.h"

#include "mathUtils/geometry.h"
#include "gridApproximizer.h"

using namespace qReal::layouts;

GridLayoutHandler::GridLayoutHandler(QGraphicsWidget *layoutHost)
	: LayoutHandler(layoutHost)
	, mLayout(NULL)
{
}

void GridLayoutHandler::insertItem(QGraphicsLayoutItem *item, int row, int column)
{
	mLayout->addItem(item, row, column, Qt::AlignCenter);
}

void GridLayoutHandler::addItemTo(QGraphicsLayoutItem *item, QPointF const &pos)
{
	// Placement algorthm:
	// 1. If there are no items at all placing into (0,0).
	// 2. Counting grid map: the coordinates of rows and columns.
	// 3. There can happen situation that user wants to place
	//    item before the first row or the first column. Then
	//    we must move items correspondingly mLayout->addItem to
	//    work correctly. Processing this situation separately.
	// 4. Searching for closest non-occupied cell. Using
	//    distance between rect and point for this purpose.
	// 5. There can be problems with cells occupied by placeholder.
	//    To eliminate them we just consider that a cell with
	//    placeholder is a free one.
	if (mLayout->rowCount() == 0 && mLayout->columnCount() == 0) {
		insertItem(item, 0, 0);
		return;
	}

	rebuildGridMap();

	int firstOccupiedRow = 0, firstOccupiedColumn = 0;
	if (processShiftCase(item, pos, firstOccupiedRow, firstOccupiedColumn)) {
		// Item added there, nothing to do here
		return;
	}

	qreal minDistance = veryLargeNumber();
	int closestRow, closestColumn;

	for (int row = firstOccupiedRow; row < mLayout->rowCount(); ++row) {
		for (int column = firstOccupiedColumn; column < mLayout->columnCount(); ++column) {
			if (!isCellFree(row, column)) {
				continue;
			}
			QRectF const cellRect(mColumnMap[column], mRowMap[row]
					, mColumnMap[column + 1] - mColumnMap[column]
					, mRowMap[row + 1] - mRowMap[row]);
			minimizeDistance(cellRect, pos, minDistance
					, closestRow, closestColumn, row, column);
		}
	}

	// Counting last row and column cells distances
	int const lastRow = mLayout->rowCount();
	int const lastColumn = mLayout->columnCount();
	for (int row = firstOccupiedRow; row < mLayout->rowCount(); ++row) {
		// Else we get "hopping" placeholder
		if (item != placeholder() || mLayout->itemAt(row, lastColumn - 1) != item) {
			QRectF const cellRect(mColumnMap[lastColumn], mRowMap[row]
					, mItem->geometry().right() - mColumnMap[lastColumn]
					, mRowMap[row + 1] - mRowMap[row]);
			minimizeDistance(cellRect, pos, minDistance
					, closestRow, closestColumn, row, lastColumn);
		}
	}
	for (int column = firstOccupiedColumn; column < mLayout->columnCount(); ++column) {
		// Else we get "hopping" placeholder
		if (item != placeholder() || mLayout->itemAt(lastRow - 1, column) != item) {
			QRectF const cellRect(mColumnMap[column], mRowMap[lastRow]
					, mColumnMap[column + 1] - mColumnMap[column]
					, mItem->geometry().bottom() - mRowMap[lastRow]);
			minimizeDistance(cellRect, pos, minDistance
					, closestRow, closestColumn, lastRow, column);
		}
	}

	// Else we get "hopping" placeholder
	if (item != placeholder() || mLayout->itemAt(lastRow - 1, lastColumn - 1) != item) {
		QRectF const lastCellRect(mColumnMap[lastColumn], mRowMap[lastRow]
				, mItem->geometry().right() - mColumnMap[lastColumn]
				, mItem->geometry().bottom() - mRowMap[lastRow]);
		minimizeDistance(lastCellRect, pos, minDistance, closestRow, closestColumn
				, lastRow, lastColumn);
	}

	insertItem(item, closestRow, closestColumn);
}

void GridLayoutHandler::minimizeDistance(QRectF const &cell
		, QPointF const &position, qreal &currentMinimum
		, int &closestRow, int &closestColumn
		, int currentRow, int currentColumn)
{
	qreal const distance = mathUtils::Geometry::distance(cell, position);
	if (distance < currentMinimum) {
		currentMinimum = distance;
		closestRow = currentRow;
		closestColumn = currentColumn;
	}
}

void GridLayoutHandler::removeItem(QGraphicsLayoutItem *item)
{
	if (containsItem(item)) {
		mLayout->removeItem(item);
	}
}

QGraphicsLayout *GridLayoutHandler::generateLayout()
{
	return mLayout = new QGraphicsGridLayout;
}

void GridLayoutHandler::placeChildrenWithoutLayout()
{
	if (!mLayout) {
		return;
	}
	QList<QGraphicsLayoutItem *> childrenToPlace = childrenWithoutLayout();
	GridApproximizer::placeItems(mLayout, childrenToPlace);
}

qreal GridLayoutHandler::veryLargeNumber() const
{
	return 1e10;
}

bool GridLayoutHandler::isCellFree(int row, int column) const
{
	return row >= mLayout->rowCount()
			|| column > mLayout->columnCount()
			|| !mLayout->itemAt(row, column)
			|| mLayout->itemAt(row, column) == placeholder();
}

bool GridLayoutHandler::isRowEmpty(int row) const
{
	for (int column = 0; column < mLayout->columnCount(); ++column) {
		if (!isCellFree(row, column)) {
			return false;
		}
	}
	return true;
}

bool GridLayoutHandler::isColumnEmpty(int column) const
{
	for (int row = 0; row < mLayout->rowCount(); ++row) {
		if (!isCellFree(row, column)) {
			return false;
		}
	}
	return true;
}

bool GridLayoutHandler::processShiftCase(QGraphicsLayoutItem *item
		, QPointF const &position
		, int &firstOccupiedRow
		, int &firstOccupiedColumn)
{
	// If there happened that we have empty first row or column
	// (for example, placeholder there) we do not want to
	// increase their count but to reuse them
	firstOccupiedRow = 0;
	while (firstOccupiedRow < mLayout->rowCount() && isRowEmpty(firstOccupiedRow)) {
		++firstOccupiedRow;
	}
	if (firstOccupiedRow >= mLayout->rowCount()) {
		// No need to check columns, the layout is empty everywhere
		insertItem(item, 0, 0);
		return true;
	}

	firstOccupiedColumn = 0;
	while (firstOccupiedColumn < mLayout->columnCount() && isColumnEmpty(firstOccupiedColumn)) {
		++firstOccupiedColumn;
	}
	// 100% met item, no need to check

	int rowShift = 0, columnShift = 0;
	bool shouldProcessRows = false, shouldProcessColumns = false;

	qreal const firstRowBorder = mRowMap[firstOccupiedRow];
	if (position.y() < firstRowBorder) {
		// Then we just in described sutiation, processing addition here
		shouldProcessRows = true;
		if (firstOccupiedRow == 0) {
			rowShift = 1;
		}
	}
	qreal const firstColumnBorder = mColumnMap[firstOccupiedColumn];
	if (position.x() < firstColumnBorder) {
		// Then we just in described sutiation, processing addition here
		shouldProcessColumns = true;
		if (firstOccupiedColumn == 0) {
			columnShift = 1;
		}
	}

	if (!shouldProcessRows && !shouldProcessColumns) {
		return false;
	}

	shiftItems(rowShift, columnShift);

	int const targetRow = shouldProcessRows ? 0 : findPosition(position.y()
			, mRowMap, firstOccupiedRow);
	int const targetColumn = shouldProcessColumns ? 0 : findPosition(position.x()
			, mColumnMap, firstOccupiedColumn);

	insertItem(item, targetRow, targetColumn);
	return true;
}

void GridLayoutHandler::rebuildGridMap()
{
	if (mLayout) {
		rebuildRowsMap();
		rebuildColumnsMap();
	}
}

void GridLayoutHandler::rebuildRowsMap()
{
	mRowMap.clear();

	for (int row = 0; row < mLayout->rowCount(); ++row) {
		int curRow = INT_MAX;
		for (int column = 0; column < mLayout->columnCount(); ++column) {
			QGraphicsLayoutItem *item = mLayout->itemAt(row, column);
			if (item && item->geometry().top() < curRow) {
				curRow = item->geometry().top();
			}
		}
		mRowMap << curRow;
	}
	int lastRow = 0;
	int const lastRowNum = mLayout->rowCount() - 1;
	for (int column = 0; column < mLayout->columnCount(); ++column) {
		QGraphicsLayoutItem *item = mLayout->itemAt(lastRowNum, column);
		if (item && item->geometry().bottom() > lastRow) {
			lastRow = item->geometry().bottom();
		}
	}
	mRowMap << lastRow;
}

void GridLayoutHandler::rebuildColumnsMap()
{
	mColumnMap.clear();

	for (int column = 0; column < mLayout->columnCount(); ++column) {
		int curColumn = INT_MAX;
		for (int row = 0; row < mLayout->rowCount(); ++row) {
			QGraphicsLayoutItem *item = mLayout->itemAt(row, column);
			if (item && item->geometry().left() < curColumn) {
				curColumn = item->geometry().left();
			}
		}
		mColumnMap << curColumn;
	}
	int lastColumn = 0;
	int const lastColumnNum = mLayout->columnCount() - 1;
	for (int row = 0; row < mLayout->rowCount(); ++row) {
		QGraphicsLayoutItem *item = mLayout->itemAt(row, lastColumnNum);
		if (item && item->geometry().right() > lastColumn) {
			lastColumn = item->geometry().right();
		}
	}
	mColumnMap << lastColumn;
}

int GridLayoutHandler::findPosition(qreal const point
		, QVector<qreal> const &map
		, int searchStart) const
{
	for (int pos = searchStart; pos < map.count() - 1; ++pos) {
		if (point < map[pos + 1]) {
			return pos;
		}
	}
	return map.count() - 1;
}

void GridLayoutHandler::shiftItems(int rowShift, int columnShift)
{
	if (rowShift == 0 && columnShift == 0) {
		return;
	}
	int const currentRowCount = mLayout->rowCount();
	int const currentColumnCount = mLayout->columnCount();
	for (int row = currentRowCount - 1; row >= 0 ; --row) {
		for (int column = currentColumnCount - 1; column >= 0 ; --column) {
			QGraphicsLayoutItem *shiftedItem = mLayout->itemAt(row, column);
			if (!isCellFree(row, column)) {
				insertItem(shiftedItem, row + rowShift, column + columnShift);
			}
		}
	}
}

bool GridLayoutHandler::containsItem(QGraphicsLayoutItem const *item) const
{
	if (!item) {
		return false;
	}
	for (int row = 0; row < mLayout->rowCount(); ++row) {
		for (int column = 0; column < mLayout->columnCount(); ++column) {
			if (mLayout->itemAt(row, column) == item) {
				return true;
			}
		}
	}
	return false;
}
