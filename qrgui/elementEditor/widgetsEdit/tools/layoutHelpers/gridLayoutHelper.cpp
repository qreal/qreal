#include <QtCore/QDebug>
#include <QtGui/QPainter>
#include <QtGui/QWidget>

#include "gridLayoutHelper.h"

using namespace qReal::widgetsEdit;

GridLayoutHelper::GridLayoutHelper(LayoutTool *tool)
	: LayoutHelperBase(tool), mTool(tool), mLayout(NULL)
{
}

void GridLayoutHelper::drawCurrentDropPosition(QPainter *painter)
{
	QRectF const rect = determineCell();

	QPen pen;
	pen.setBrush(Qt::blue);
	pen.setWidth(3);
	pen.setCapStyle(Qt::RoundCap);

	painter->save();
	painter->setPen(pen);
	painter->drawRect(rect);
	painter->restore();
}

void GridLayoutHelper::dropItem(Tool *item)
{
	insertItem(item, mCurrentRow - 1, mCurrentColumn - 1);
	mLayout->activate();
	updateGrid();
}

void GridLayoutHelper::resetLayout(QGraphicsLayout *layout)
{
	LayoutHelperBase::resetLayout(layout);

	mTool->setLayout(NULL);
	mLayout = dynamic_cast<QGraphicsGridLayout *>(layout);
	if (!mLayout) {
		return;
	}
	int row = 0;
	foreach (QGraphicsItem *child, mTool->childItems()) {
		Tool *tool = dynamic_cast<Tool *>(child);
		if (!tool) {
			continue;
		}
		insertItem(tool, row++, 0);
	}
	mTool->setLayout(mLayout);
	mLayout->setPreferredSize(mTool->geometry().width(), mTool->geometry().height());
	mTool->setMinimumSize(mLayout->minimumSize());

	updateGrid();
}

void GridLayoutHelper::startChildDrag(Tool *item)
{
	mDraggedChild = item;

	for (int row = 0; row < mLayout->rowCount(); ++row) {
		for (int column = 0; column < mLayout->columnCount(); ++column) {
			if (mLayout->itemAt(row, column) == item) {
				mDraggedChildOldRow = row;
				mDraggedChildOldColumn = column;

				int index = 0;
				while (mLayout->itemAt(index) != item) {
					index++;
				}
				mLayout->removeAt(index);
				updateGrid();

				return;
			}
		}
	}
}

void GridLayoutHelper::finishChildDrag(bool success)
{
	if (!success) {
		insertItem(mDraggedChild, mDraggedChildOldRow, mDraggedChildOldColumn);
	}
}

void GridLayoutHelper::generateAttachedProperty(QDomElement &element, Tool *tool)
{
	QPair<int, int> const index = findTool(tool);
	element.setAttribute("row", index.first);
	element.setAttribute("column", index.second);
}

void GridLayoutHelper::onToolResized(const QRectF &newGeometry)
{
	mLayout->setPreferredSize(newGeometry.width(), newGeometry.height());
	updateGrid();
}

QRectF GridLayoutHelper::determineCell()
{
	determineRow();
	determineColumn();

	qreal top, left, bottom, right;

	if (mCurrentRow == 0 || mCurrentColumn == 0) {
		top = mCurrentRow == 0 ? 0 : mRows[mCurrentRow - 1];
		left = mCurrentColumn == 0 ? 0 : mColumns[mCurrentColumn - 1];
		bottom = (mCurrentRow == mRows.count())
				? mLayout->geometry().bottom()
				: mRows[mCurrentRow];
		right = (mCurrentColumn == mColumns.count())
				? mLayout->geometry().right()
				: mColumns[mCurrentColumn];
		return QRectF(left, top, right - left, bottom - top);
	}

	// Moving forward by columns while the cell is occupied.
	// This behaviour is hardcoded, it is incorrect.
	// TODO: think about correct behaviour
	while (mCurrentColumn < mColumns.count() &&
			mLayout->itemAt(mCurrentRow - 1, mCurrentColumn - 1)) {
		++mCurrentColumn;
	}

	top = mRows[mCurrentRow - 1];
	left = mColumns[mCurrentColumn - 1];
	bottom = (mCurrentRow == mRows.count())
			? mLayout->geometry().bottom()
			: mRows[mCurrentRow];
	right = (mCurrentColumn == mColumns.count())
			? mLayout->geometry().right()
			: mColumns[mCurrentColumn];
	return QRectF(left, top, right - left, bottom - top);
}

void GridLayoutHelper::determineRow()
{
	mCurrentRow = 0;
	while (mCurrentRow < mRows.count() &&
			mRows[mCurrentRow] <= mPos.y()) {
		++mCurrentRow;
	}
}

void GridLayoutHelper::determineColumn()
{
	mCurrentColumn = 0;
	while (mCurrentColumn < mColumns.count() &&
			mColumns[mCurrentColumn] <= mPos.x()) {
		++mCurrentColumn;
	}
}

void GridLayoutHelper::insertItem(Tool *item, int row, int column)
{
	if (row < 0 || column < 0) {
		QGraphicsGridLayout *newLayout = new QGraphicsGridLayout;

		int rowShift = 0;
		int columnShift = 0;
		if (row < 0) {
			row = 0;
			rowShift = 1;
		}
		if (column < 0) {
			column = 0;
			columnShift = 1;
		}
		newLayout->addItem(item, row, column);
		newLayout->setAlignment(item, Qt::AlignCenter);

		for (int r = 0; r < mLayout->rowCount(); ++r) {
			for (int c = 0; c < mLayout->columnCount(); ++c) {
				QGraphicsLayoutItem *copiedItem = mLayout->itemAt(r, c);
				if (copiedItem) {
					newLayout->addItem(copiedItem, r + rowShift
						, c + columnShift, Qt::AlignCenter);
				}
			}
			newLayout->setRowAlignment(r, Qt::AlignCenter);
		}

		mLayout = newLayout;
		mTool->setLayout(newLayout);
		mLayout->setPreferredSize(mTool->geometry().width(), mTool->geometry().height());
		mTool->setMinimumSize(newLayout->minimumSize());
	} else {
		mLayout->addItem(item, row, column);
		mLayout->setRowAlignment(row, Qt::AlignCenter);
		mLayout->setAlignment(item, Qt::AlignCenter);
	}
}

void GridLayoutHelper::updateGrid()
{
	if (!mLayout) {
		return;
	}

	updateRows();
	updateColumns();
}

void GridLayoutHelper::updateRows()
{
	mRows.clear();

	for (int row = 0; row < mLayout->rowCount(); ++row) {
		int curRow = INT_MAX;
		for (int column = 0; column < mLayout->columnCount(); ++column) {
			QGraphicsLayoutItem *item = mLayout->itemAt(row, column);
			if (item && item->geometry().top() < curRow) {
				curRow = item->geometry().top();
			}
		}
		mRows << curRow;
	}
	int lastRow = 0;
	int const lastRowNum = mLayout->rowCount() - 1;
	for (int column = 0; column < mLayout->columnCount(); ++column) {
		QGraphicsLayoutItem *item = mLayout->itemAt(lastRowNum, column);
		if (item && item->geometry().bottom() > lastRow) {
			lastRow = item->geometry().bottom();
		}
	}
	mRows << lastRow;
}

void GridLayoutHelper::updateColumns()
{
	mColumns.clear();

	for (int column = 0; column < mLayout->columnCount(); ++column) {
		int curColumn = INT_MAX;
		for (int row = 0; row < mLayout->rowCount(); ++row) {
			QGraphicsLayoutItem *item = mLayout->itemAt(row, column);
			if (item && item->geometry().left() < curColumn) {
				curColumn = item->geometry().left();
			}
		}
		mColumns << curColumn;
	}
	int lastColumn = 0;
	int const lastColumnNum = mLayout->columnCount() - 1;
	for (int row = 0; row < mLayout->rowCount(); ++row) {
		QGraphicsLayoutItem *item = mLayout->itemAt(row, lastColumnNum);
		if (item && item->geometry().right() > lastColumn) {
			lastColumn = item->geometry().right();
		}
	}
	mColumns << lastColumn;
}

QPair<int, int> GridLayoutHelper::findTool(Tool *tool) const
{
	for (int i = 0; i < mRows.count(); ++i) {
		for (int j = 0; j < mColumns.count(); ++j) {
			if (tool == mLayout->itemAt(i, j)) {
				return QPair<int, int>(i, j);
			}
		}
	}
	return QPair<int, int>(-1, -1);
}
