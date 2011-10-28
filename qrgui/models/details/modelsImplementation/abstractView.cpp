#include "abstractView.h"

#include "abstractModel.h"
#include "../../qrkernel/definitions.h"

using namespace qReal;
using namespace models::details::modelsImplementation;

AbstractView::AbstractView(AbstractModel * const model)
	: mModel(model)
{
}

void AbstractView::rowsAboutToBeMoved(QModelIndex const &sourceParent
		, int sourceStart, int sourceEnd, QModelIndex const &destinationParent, int destinationRow)
{
	Q_UNUSED(sourceParent)
	Q_UNUSED(sourceStart)
	Q_UNUSED(sourceEnd)
	Q_UNUSED(destinationParent)
	Q_UNUSED(destinationRow)
}

void AbstractView::rowsMoved(QModelIndex const &sourceParent, int sourceStart
		, int sourceEnd, QModelIndex const &destinationParent, int destinationRow)
{
	Q_UNUSED(sourceParent)
	Q_UNUSED(sourceStart)
	Q_UNUSED(sourceEnd)
	Q_UNUSED(destinationParent)
	Q_UNUSED(destinationRow)
}

void AbstractView::dataChanged(QModelIndex const &topLeft, QModelIndex const &bottomRight)
{
	Q_UNUSED(topLeft)
	Q_UNUSED(bottomRight)
}

void AbstractView::rowsAboutToBeRemoved(QModelIndex const &parent, int start, int end)
{
	Q_UNUSED(parent)
	Q_UNUSED(start)
	Q_UNUSED(end)
}

void AbstractView::rowsInserted(QModelIndex const &parent, int start, int end)
{
	Q_UNUSED(parent)
	Q_UNUSED(start)
	Q_UNUSED(end)
}

QModelIndex AbstractView::indexAt(const QPoint &point) const
{
	Q_UNUSED(point)
	return QModelIndex();
}

QRect AbstractView::visualRect(const QModelIndex &index) const
{
	Q_UNUSED(index)
	return QRect();
}

void AbstractView::scrollTo(const QModelIndex &index, ScrollHint hint)
{
	Q_UNUSED(index);
	Q_UNUSED(hint);
}

bool AbstractView::isDescendentOf(const QModelIndex &descendent, const QModelIndex &ancestor)
{
	Q_UNUSED(descendent)
	Q_UNUSED(ancestor)
	return false;
}

QModelIndex AbstractView::moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
	Q_UNUSED(cursorAction)
	Q_UNUSED(modifiers)
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
	Q_UNUSED(index)
	return false;
}

void AbstractView::setSelection(const QRect& rect, QItemSelectionModel::SelectionFlags command)
{
	Q_UNUSED(rect)
	Q_UNUSED(command)
}

QRegion AbstractView::visualRegionForSelection(const QItemSelection &selection ) const
{
	Q_UNUSED(selection)
	return QRegion();
}
