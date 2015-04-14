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

#include "logicalModelView.h"

#include "models/details/graphicalModel.h"

using namespace qReal;
using namespace models::details;
using namespace modelsImplementation;

LogicalModelView::LogicalModelView(GraphicalModel * const model)
	: AbstractView(model)
{
}

void LogicalModelView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight
		, QVector<int> const &roles)
{
	Q_UNUSED(roles)
	for (int row = topLeft.row(); row <= bottomRight.row(); ++row) {
		QModelIndex curr = topLeft.sibling(row, 0);

		const Id logicalId = curr.data(roles::idRole).value<Id>();
		static_cast<GraphicalModel *>(mModel)->updateElements(logicalId, curr.data(Qt::DisplayRole).toString());
	}
}

void LogicalModelView::rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end)
{
	for (int row = start; row <= end; ++row) {
		QModelIndex current = model()->index(row, 0, parent);
		if (current.isValid()) {
			const Id logicalId = current.data(roles::idRole).value<Id>();
			GraphicalModel *gModel = dynamic_cast<GraphicalModel *>(mModel);
			QList<QPersistentModelIndex> indexes = gModel->indexesWithLogicalId(logicalId);
			foreach (QPersistentModelIndex index, indexes) {
				gModel->removeRows(index.row(), 1, index.parent());
			}
		}
	}
	QAbstractItemView::rowsAboutToBeRemoved(parent, start, end);
}
