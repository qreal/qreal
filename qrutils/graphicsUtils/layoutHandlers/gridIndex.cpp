#include "gridIndex.h"

using namespace qReal::layouts;

GridIndex::GridIndex()
	: mRow(0), mColumn(0), mHorizontalSpan(0), mVerticalSpan(0), mBindedItem(NULL)
{
}

GridIndex::GridIndex(int row, int column, int hSpan, int vSpan)
	: mRow(row), mColumn(column), mHorizontalSpan(hSpan), mVerticalSpan(vSpan)
	, mBindedItem(NULL)
{
}

int GridIndex::row() const
{
	return mRow;
}

int GridIndex::column() const
{
	return mColumn;
}

int GridIndex::horizontalSpan() const
{
	return mHorizontalSpan;
}

int GridIndex::verticalSpan() const
{
	return mVerticalSpan;
}

void GridIndex::place(QGraphicsGridLayout * const target
		, QGraphicsLayoutItem * const item, Qt::Alignment alignment)
{
	target->addItem(item, mRow, mColumn, mVerticalSpan, mHorizontalSpan, alignment);
}

void GridIndex::place(QGraphicsGridLayout * const target, Qt::Alignment alignment)
{
	if (mBindedItem) {
		place(target, mBindedItem, alignment);
	}
}

bool GridIndex::operator<(GridIndex const &other) const
{
	return mRow == other.mRow ? mColumn < other.mColumn : mRow < other.mRow;
}

void GridIndex::bindWithItem(QGraphicsLayoutItem * const item)
{
	mBindedItem = item;
}
