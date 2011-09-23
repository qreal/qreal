#include "logicalModelView.h"

#include "graphicalModel.h"

#include <QtCore/QDebug>

using namespace qReal;
using namespace models::details;
using namespace modelsImplementation;

LogicalModelView::LogicalModelView(GraphicalModel * const model)
	: AbstractView(model)
{
}

void LogicalModelView::dataChanged(QModelIndex const &topLeft, QModelIndex const &bottomRight)
{
	for (int row = topLeft.row(); row <= bottomRight.row(); ++row) {
		QModelIndex curr = topLeft.sibling(row, 0);

		Id const logicalId = curr.data(roles::idRole).value<Id>();
		static_cast<GraphicalModel *>(mModel)->updateElements(logicalId, curr.data(Qt::DisplayRole).toString());
	}
}

void LogicalModelView::rowsAboutToBeRemoved(QModelIndex const &parent, int start, int end)
{
	for (int row = start; row <= end; ++row) {
		QModelIndex current = model()->index(row, 0, parent);
		if (current.isValid()) {
			Id const logicalId = current.data(roles::idRole).value<Id>();
			GraphicalModel *gModel = dynamic_cast<GraphicalModel *>(mModel);
			QList<QPersistentModelIndex> indexes = gModel->indexesWithLogicalId(logicalId);
			foreach (QPersistentModelIndex index, indexes) {
				gModel->removeRows(index.row(), 1, index.parent());
			}
		}
	}
	QAbstractItemView::rowsAboutToBeRemoved(parent, start, end);
}

