/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
