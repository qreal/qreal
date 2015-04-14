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

#include "graphicalPartViewMock.h"

#include <models/details/modelsImplementation/graphicalPartModelItem.h>

using namespace qrguiTests::helpers;
using namespace qReal;
using namespace qReal::models::details::modelsImplementation;

GraphicalPartViewMock::GraphicalPartViewMock()
	: mDataChangedCalledWithIndex(-1)
	, mRowsAboutToBeRemovedCalledWithRow(-1)
	, mRowsInsertedCalledWithIndex(-1)
{
}

QModelIndex GraphicalPartViewMock::indexAt(QPoint const &point) const
{
	Q_UNUSED(point);

	return QModelIndex();
}

void GraphicalPartViewMock::scrollTo(QModelIndex const &index, ScrollHint hint)
{
	Q_UNUSED(index)
	Q_UNUSED(hint)
}

QRect GraphicalPartViewMock::visualRect(QModelIndex const &index) const
{
	Q_UNUSED(index)

	return QRect();
}

void GraphicalPartViewMock::dataChanged(QModelIndex const &topLeft, QModelIndex const &bottomRight)
{
	if (topLeft != bottomRight) {
		return;
	}

	GraphicalPartModelItem const * const item = static_cast<GraphicalPartModelItem const *>(topLeft.internalPointer());
	if (!item) {
		return;
	}

	mDataChangedCalledWithId = item->id();
	mDataChangedCalledWithIndex = item->index();
}

void GraphicalPartViewMock::rowsAboutToBeRemoved(QModelIndex const &parent, int start, int end)
{
	Q_UNUSED(parent)

	if (start != end) {
		return;
	}

	mRowsAboutToBeRemovedCalledWithRow = start;
}

void GraphicalPartViewMock::rowsInserted(QModelIndex const &parent, int start, int end)
{
	if (start != end) {
		return;
	}

	if (!parent.isValid()) {
		mRowsInsertedCalledWithIndexFirstTime = start;
		return;
	}

	GraphicalPartModelItem const * const item
			= static_cast<GraphicalPartModelItem const *>(parent.child(start, 0).internalPointer());
	if (!item) {
		return;
	}

	mRowsInsertedCalledWithId = item->id();
	mRowsInsertedCalledWithIndex = item->index();
}

QModelIndex GraphicalPartViewMock::moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
	Q_UNUSED(cursorAction)
	Q_UNUSED(modifiers)

	return QModelIndex();
}

int GraphicalPartViewMock::horizontalOffset() const
{
	return 0;
}

int GraphicalPartViewMock::verticalOffset() const
{
	return 0;
}

bool GraphicalPartViewMock::isIndexHidden(QModelIndex const &index) const
{
	Q_UNUSED(index);

	return false;
}

void GraphicalPartViewMock::setSelection(QRect const &rect, QItemSelectionModel::SelectionFlags command)
{
	Q_UNUSED(rect)
	Q_UNUSED(command)
}

QRegion GraphicalPartViewMock::visualRegionForSelection(QItemSelection const &selection) const
{
	Q_UNUSED(selection)

	return QRegion();
}

qReal::Id const &GraphicalPartViewMock::dataChangedCalledWithId() const
{
	return mDataChangedCalledWithId;
}

int const &GraphicalPartViewMock::dataChangedCalledWithIndex() const
{
	return mDataChangedCalledWithIndex;
}

int const &GraphicalPartViewMock::rowsAboutToBeRemovedCalledWithRow() const
{
	return mRowsAboutToBeRemovedCalledWithRow;
}

qReal::Id const &GraphicalPartViewMock::rowsInsertedCalledWithId() const
{
	return mRowsInsertedCalledWithId;
}

int const &GraphicalPartViewMock::rowsInsertedCalledWithIndex() const
{
	return mRowsInsertedCalledWithIndex;
}

int const &GraphicalPartViewMock::rowsInsertedCalledWithIndexFirstTime() const
{
	return mRowsInsertedCalledWithIndexFirstTime;
}
