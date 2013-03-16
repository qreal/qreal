#pragma once

#include <QtGui/QGraphicsGridLayout>

#include "../../utilsDeclSpec.h"

namespace qReal
{
namespace layouts
{

/// Represents some position in grid layout
struct QRUTILS_EXPORT GridIndex
{
public:
	GridIndex();
	GridIndex(int row, int column, int hSpan, int vSpan);

	int row() const;
	int column() const;
	int horizontalSpan() const;
	int verticalSpan() const;

	/// Places @param item into @param target
	void place(QGraphicsGridLayout * const target
		, QGraphicsLayoutItem * const item
		, Qt::Alignment alignment = Qt::AlignCenter);

	/// Places binded item into @param target
	void place(QGraphicsGridLayout * const target
		, Qt::Alignment alignment = Qt::AlignCenter);

	/// Straight order: (0,0) < (0,1) < (0,2) < ... < (1,0) < ...
	bool operator<(GridIndex const &other) const;

	/// Associates this index with item. The item will be used in one
	/// of the place() overload
	void bindWithItem(QGraphicsLayoutItem * const item);

private:
	int mRow;
	int mColumn;
	int mHorizontalSpan;
	int mVerticalSpan;
	QGraphicsLayoutItem *mBindedItem;
};

}
}
