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

#include "abstractView.h"

#include <qrkernel/definitions.h>

#include "models/details/modelsImplementation/abstractModel.h"

using namespace qReal;
using namespace models::details::modelsImplementation;

AbstractView::AbstractView(AbstractModel * const model)
	: mModel(model)
{
}

AbstractView::~AbstractView()
{
}

void AbstractView::rowsAboutToBeMoved(const QModelIndex &sourceParent
		, int sourceStart, int sourceEnd, const QModelIndex &destinationParent, int destinationRow)
{
	Q_UNUSED(sourceParent)
	Q_UNUSED(sourceStart)
	Q_UNUSED(sourceEnd)
	Q_UNUSED(destinationParent)
	Q_UNUSED(destinationRow)
}

void AbstractView::rowsMoved(const QModelIndex &sourceParent, int sourceStart
		, int sourceEnd, const QModelIndex &destinationParent, int destinationRow)
{
	Q_UNUSED(sourceParent)
	Q_UNUSED(sourceStart)
	Q_UNUSED(sourceEnd)
	Q_UNUSED(destinationParent)
	Q_UNUSED(destinationRow)
}

void AbstractView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight
		, QVector<int> const &roles)
{
	Q_UNUSED(topLeft)
	Q_UNUSED(bottomRight)
	Q_UNUSED(roles)
}

void AbstractView::rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end)
{
	Q_UNUSED(parent)
	Q_UNUSED(start)
	Q_UNUSED(end)
}

void AbstractView::rowsInserted(const QModelIndex &parent, int start, int end)
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
