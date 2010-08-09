#include "logicalModelView.h"

#include "graphicalModel.h"

using namespace qReal;
using namespace models;
using namespace details;

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
