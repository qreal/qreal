#include "graphicalModelView.h"

#include <QtCore/QUuid>
#include <QtCore/QDebug>

#include "logicalModel.h"

using namespace qReal;
using namespace models::details;
using namespace modelsImplementation;

GraphicalModelView::GraphicalModelView(LogicalModel * const model)
	: AbstractView(model)
{
}

void GraphicalModelView::rowsInserted(QModelIndex const &parent, int start, int end)
{
	QPersistentModelIndex const parentIndex = parent.sibling(parent.row(), 0);
	Id const parentLogicalId = parentIndex.data(roles::logicalIdRole).value<Id>();

	for (int row = start; row <= end; ++row) {
		QPersistentModelIndex const current = model()->index(row, 0, parent);
		Id const logicalId = current.data(roles::logicalIdRole).value<Id>();
		if (parentLogicalId == Id())
			return;
		QString const name = current.data(Qt::DisplayRole).toString();
		if (logicalId == Id())
			// No logical Id for this item, so logical model shouldn't care
			// about it.
			continue;

		// Add this element to a root for now. To be able to do something
		// useful, we need to establish a correspondence between logical
		// and graphical model hierarchy. It is not always easy since
		// some elements have no correspondences in another model, and tree
		// structures may be very different by themselves.
		LogicalModel * const mLogicalModel = static_cast<LogicalModel *>(mModel);
		mLogicalModel->addElementToModel(parentLogicalId, logicalId, logicalId, name, QPoint(0, 0));
	}
}

void GraphicalModelView::dataChanged(QModelIndex const &topLeft, QModelIndex const &bottomRight)
{
	for (int row = topLeft.row(); row <= bottomRight.row(); ++row) {
		QModelIndex current = topLeft.sibling(row, 0);

		Id const logicalId = current.data(roles::logicalIdRole).value<Id>();
		static_cast<LogicalModel *>(mModel)->updateElements(logicalId, current.data(Qt::DisplayRole).toString());
	}
	Id const parentLogicalId = topLeft.sibling(topLeft.row(), 0).data(roles::logicalIdRole).value<Id>();
	Id const childLogicalId = bottomRight.sibling(bottomRight.row(), 0).data(roles::logicalIdRole).value<Id>();
	if (parentLogicalId != childLogicalId)
		static_cast<LogicalModel *>(mModel)->changeParent(parentLogicalId, childLogicalId);
}

void GraphicalModelView::rowsAboutToBeRemoved(QModelIndex const &parent, int start, int end)
{
	Q_UNUSED(parent);
	Q_UNUSED(start);
	Q_UNUSED(end);
}


