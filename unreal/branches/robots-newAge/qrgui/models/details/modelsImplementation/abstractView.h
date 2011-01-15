#pragma once

#include <QtGui/QAbstractItemView>

#include "abstractModel.h"

namespace qReal {

namespace models {

namespace details {

namespace modelsImplementation {

class AbstractView : public QAbstractItemView
{
	Q_OBJECT
public:
	AbstractView(AbstractModel * const model);

public slots:
	void rowsAboutToBeMoved(QModelIndex const &sourceParent, int sourceStart, int sourceEnd, QModelIndex const &destinationParent, int destinationRow);
	void rowsMoved(QModelIndex const &sourceParent, int sourceStart, int sourceEnd, QModelIndex const &destinationParent, int destinationRow);

protected slots:
	void dataChanged(QModelIndex const &topLeft, QModelIndex const &bottomRight);
	virtual void rowsAboutToBeRemoved(QModelIndex const &parent, int start, int end);
	void rowsInserted(QModelIndex const &parent, int start, int end);

protected:
	AbstractModel * const mModel;

	// Unneeded stuff from QAbstractItemView. It would be helpful if Qt Model/View framework
	// were moved to QtCore and got separated from GUI concerns, since Model/View pattern
	// is more general.
	QModelIndex indexAt(const QPoint &point) const;
	QRect visualRect(const QModelIndex &index) const;
	void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible);
	bool isDescendentOf(const QModelIndex &descendent, const QModelIndex &ancestor);
	QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers);
	int horizontalOffset() const;
	int verticalOffset() const;
	bool isIndexHidden(const QModelIndex &index) const;
	void setSelection(const QRect& rect, QItemSelectionModel::SelectionFlags command);
	QRegion visualRegionForSelection(const QItemSelection &selection ) const;
};

}

}

}

}
