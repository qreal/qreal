#include "linearLayoutHandler.h"

using namespace qReal::layouts;

LinearLayoutHandler::LinearLayoutHandler(Qt::Orientation orientation
		, QGraphicsWidget *layoutHost)
	: LayoutHandler(layoutHost)
	, mOrientation(orientation)
	, mLayout(NULL)
{
}

void LinearLayoutHandler::addItemTo(QGraphicsLayoutItem *item, QPointF const &pos)
{
	if (!mLayout) {
		return;
	}
	int const cell = cellToAdd(pos);
	int const cellToCheck = qMin(mLayout->count() - 1, cell);
	if (item && (cellToCheck < 0 || mLayout->itemAt(cellToCheck) != item)) {
		removeItem(item);
		mLayout->insertItem(cell, item);
		mLayout->setAlignment(item, Qt::AlignCenter);
		mLayout->activate();
	}
}

void LinearLayoutHandler::removeItem(QGraphicsLayoutItem *item)
{
	mLayout->removeItem(item);
}

QGraphicsLayout *LinearLayoutHandler::generateLayout()
{
	mLayout = new QGraphicsLinearLayout(mOrientation);
	return mLayout;
}

int LinearLayoutHandler::cellToAdd(QPointF const &position)
{
	int const cellsCount = mLayout->count();
	qreal const searchValue = actualDimension(position);
	int placeholdersCount = 0;
	for (int row = 0; row < cellsCount; ++row) {
		// else we get "hopping" placeholder
		if (mLayout->itemAt(row) != placeholder()) {
			QRectF const currentGeometry = mLayout->itemAt(row)->geometry();
			if (searchValue < actualDimension(currentGeometry.bottomRight())) {
				return row - placeholdersCount;
			}
		} else {
			++placeholdersCount;
		}
	}
	return cellsCount - placeholdersCount;
}

int LinearLayoutHandler::actualDimension(QPointF const &point) const
{
	return mOrientation == Qt::Horizontal ? point.x() : point.y();
}

int LinearLayoutHandler::indexOf(QGraphicsLayoutItem * const item)
{
	int const count = mLayout->count();
	for (int i = 0; i < count; ++i) {
		if (mLayout->itemAt(i) == item) {
			return i;
		}
	}
	return -1;
}

void LinearLayoutHandler::placeChildrenWithoutLayout()
{
	if (!mLayout) {
		return;
	}
	QList<QGraphicsLayoutItem *> childrenToPlace = childrenWithoutLayout();

	// Sorting items in order thay lay on plane for minimal relative
	// positions modifications
	qSort(childrenToPlace.begin(), childrenToPlace.end()
			, LinearLayoutItemComparator(mOrientation));

	foreach (QGraphicsLayoutItem *item, childrenToPlace) {
		mLayout->addItem(item);
	}
}

LinearLayoutItemComparator::LinearLayoutItemComparator(Qt::Orientation orientation)
	: mOrientation(orientation)
{
}

bool LinearLayoutItemComparator::operator()(const QGraphicsLayoutItem *left
		, const QGraphicsLayoutItem *right) const
{
	return actualDimension(left->geometry().topLeft()) <
			actualDimension(right->geometry().topLeft());
}

int LinearLayoutItemComparator::actualDimension(QPointF const &point) const
{
	return mOrientation == Qt::Horizontal ? point.x() : point.y();
}
