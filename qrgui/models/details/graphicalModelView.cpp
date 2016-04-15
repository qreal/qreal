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

#include "graphicalModelView.h"

#include <QtCore/QUuid>

#include "models/details/logicalModel.h"

using namespace qReal;
using namespace models::details;
using namespace modelsImplementation;

GraphicalModelView::GraphicalModelView(LogicalModel * const model)
	: AbstractView(model)
{
}

void GraphicalModelView::rowsInserted(const QModelIndex &parent, int start, int end)
{
	const QPersistentModelIndex parentIndex = parent.sibling(parent.row(), 0);
	Id parentLogicalId = parentIndex.data(roles::logicalIdRole).value<Id>();

	for (int row = start; row <= end; ++row) {
		const QPersistentModelIndex current = model()->index(row, 0, parent);
		const Id logicalId = current.data(roles::logicalIdRole).value<Id>();
		if (parentLogicalId.isNull()
				|| (parentLogicalId.editor() != "MetaEditor" && parentLogicalId.editor() != "ConstraintsMetamodel")
				|| (logicalId.editor() != "MetaEditor" && logicalId.editor() != "ConstraintsMetamodel"))
		{
			parentLogicalId = Id::rootId();
		}

		const QString name = current.data(Qt::DisplayRole).toString();
		if (logicalId.isNull()) {
			// No logical Id for this item, so logical model shouldn't care
			// about it.
			continue;
		}

		// Add this element to a root for now. To be able to do something
		// useful, we need to establish a correspondence between logical
		// and graphical model hierarchy. It is not always easy since
		// some elements have no correspondences in another model, and tree
		// structures may be very different by themselves.
		LogicalModel * const logicalModel = static_cast<LogicalModel *>(mModel);

		const bool isEdge = mModel->editorManagerInterface().isNodeOrEdge(logicalId.editor(), logicalId.element());

		ElementInfo elementInfo(logicalId, logicalId, parentLogicalId, Id(), {{"name", name}}, {}, Id(), isEdge);
		logicalModel->addElementToModel(elementInfo);
	}
}

void GraphicalModelView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight
		, const QVector<int> &roles)
{
	// Here we should update logical element`s names if they were changed in graphical model.
	if (!roles.contains(Qt::DisplayRole)) {
		return;
	}

	for (int row = topLeft.row(); row <= bottomRight.row(); ++row) {
		QModelIndex current = topLeft.sibling(row, 0);

		const Id logicalId = current.data(roles::logicalIdRole).value<Id>();
		static_cast<LogicalModel *>(mModel)->updateElements(logicalId, current.data(Qt::DisplayRole).toString());
	}
}

void GraphicalModelView::rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end)
{
	Q_UNUSED(parent);
	Q_UNUSED(start);
	Q_UNUSED(end);
}
