#include "abstractView.h"

#include "abstractModel.h"
#include "../kernel/definitions.h"

using namespace qReal;
using namespace models;
using namespace details;

AbstractView::AbstractView(AbstractModel * const model)
	: mModel(model)
{
}

void AbstractView::rowsAboutToBeMoved(QModelIndex const &sourceParent
		, int sourceStart, int sourceEnd, QModelIndex const &destinationParent, int destinationRow)
{

}

void AbstractView::rowsMoved(QModelIndex const &sourceParent, int sourceStart
		, int sourceEnd, QModelIndex const &destinationParent, int destinationRow)
{

}

void AbstractView::dataChanged(QModelIndex const &topLeft, QModelIndex const &bottomRight)
{

}

void AbstractView::rowsAboutToBeRemoved(QModelIndex const &parent, int start, int end)
{

}

void AbstractView::rowsInserted(QModelIndex const &parent, int start, int end)
{
}

QModelIndex AbstractView::indexAt(const QPoint &point) const
{
	return QModelIndex();
}

QRect AbstractView::visualRect(const QModelIndex &index) const
{
	return QRect();
}

void AbstractView::scrollTo(const QModelIndex &index, ScrollHint hint)
{
	Q_UNUSED(index);
	Q_UNUSED(hint);
}

bool AbstractView::isDescendentOf(const QModelIndex &descendent, const QModelIndex &ancestor)
{
	return false;
}

QModelIndex AbstractView::moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
	return QModelIndex();
}

int AbstractView::horizontalOffset() const
{
	return 0;
}

int AbstractView::verticalOffset() const
{
	return 0;
}

bool AbstractView::isIndexHidden(const QModelIndex &index) const
{
	return false;
}

void AbstractView::setSelection(const QRect& rect, QItemSelectionModel::SelectionFlags command)
{
}

QRegion AbstractView::visualRegionForSelection(const QItemSelection &selection ) const
{
	return QRegion();
}
