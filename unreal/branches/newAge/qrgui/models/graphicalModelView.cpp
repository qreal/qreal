#include "graphicalModelView.h"

#include <QtCore/QUuid>
#include <QtCore/QDebug>

#include "logicalModel.h"

using namespace qReal;
using namespace models;
using namespace details;

GraphicalModelView::GraphicalModelView(LogicalModel * const model)
	: AbstractView(model)
{
}

void GraphicalModelView::rowsInserted(QModelIndex const &parent, int start, int end)
{
	for (int row = start; row <= end; ++row) {
		QPersistentModelIndex current = model()->index(row, 0, parent);
		Id logicalId = current.data(roles::logicalIdRole).value<Id>();
		QString const name = current.data(Qt::DisplayRole).toString();
		if (logicalId == Id())
			// No logical Id for this item, so logical model shouldn't care
			// about it.
			continue;

		// Add this element to a root for now. To be able to do something
		// useful, we need to establish a correspondence between logical
		// and graphical model hierarchy. It is not always easy since
		// some elements have no corrspondences in another model, and tree
		// structures may be very different by themselves.
		LogicalModel *mLogicalModel = static_cast<LogicalModel *>(mModel);
		mLogicalModel->addElementToModel(Id::rootId(), logicalId, logicalId, name, QPoint(0, 0));
		}
	}

void GraphicalModelView::dataChanged(QModelIndex const &topLeft, QModelIndex const &bottomRight)
{
	for (int row = topLeft.row(); row <= bottomRight.row(); ++row) {
		QModelIndex current = topLeft.sibling(row, 0);

		Id const logicalId = current.data(roles::logicalIdRole).value<Id>();
		static_cast<LogicalModel *>(mModel)->updateElements(logicalId, current.data(Qt::DisplayRole).toString());
	}
}

