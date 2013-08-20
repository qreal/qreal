#pragma once

#include <QtWidgets/QAbstractItemView>

#include <../qrkernel/ids.h>

namespace qrguiTests {
namespace helpers {

class GraphicalPartViewMock : public QAbstractItemView
{
	Q_OBJECT

public:
	GraphicalPartViewMock();

	// Override.
	virtual QModelIndex indexAt(QPoint const &point) const;

	// Override.
	virtual void scrollTo(QModelIndex const &index, ScrollHint hint = EnsureVisible);

	// Override.
	virtual QRect visualRect(QModelIndex const &index) const;

	qReal::Id const &dataChangedCalledWithId() const;
	int const &dataChangedCalledWithIndex() const;
	int const &rowsAboutToBeRemovedCalledWithRow() const;
	qReal::Id const &rowsInsertedCalledWithId() const;
	int const &rowsInsertedCalledWithIndex() const;
	int const &rowsInsertedCalledWithIndexFirstTime() const;

private slots:
	virtual void dataChanged(QModelIndex const &topLeft, QModelIndex const &bottomRight);
	virtual void rowsAboutToBeRemoved(QModelIndex const &parent, int start, int end);
	virtual void rowsInserted(QModelIndex const &parent, int start, int end);

protected:
	virtual QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers);

	virtual int horizontalOffset() const;
	virtual int verticalOffset() const;

	virtual bool isIndexHidden(QModelIndex const &index) const;

	virtual void setSelection(QRect const &rect, QItemSelectionModel::SelectionFlags command);
	virtual QRegion visualRegionForSelection(QItemSelection const &selection) const;

private:
	qReal::Id mDataChangedCalledWithId;
	int mDataChangedCalledWithIndex;
	int mRowsAboutToBeRemovedCalledWithRow;
	qReal::Id mRowsInsertedCalledWithId;
	int mRowsInsertedCalledWithIndex;
	int mRowsInsertedCalledWithIndexFirstTime;
};

}
}
