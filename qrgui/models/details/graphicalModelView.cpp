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

void GraphicalModelView::rowsInserted(QModelIndex const &parent, int start, int end)
{
	QPersistentModelIndex const parentIndex = parent.sibling(parent.row(), 0);
	Id parentLogicalId = parentIndex.data(roles::logicalIdRole).value<Id>();

	for (int row = start; row <= end; ++row) {
		QPersistentModelIndex const current = model()->index(row, 0, parent);
		Id const logicalId = current.data(roles::logicalIdRole).value<Id>();
		if (parentLogicalId.isNull()
				|| (parentLogicalId.editor() != "MetaEditor" && parentLogicalId.editor() != "ConstraintsMetamodel")
				|| (logicalId.editor() != "MetaEditor" && logicalId.editor() != "ConstraintsMetamodel")) {
			parentLogicalId = Id::rootId();
		}

		QString const name = current.data(Qt::DisplayRole).toString();
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
		LogicalModel * const mLogicalModel = static_cast<LogicalModel *>(mModel);
		mLogicalModel->addElementToModel(parentLogicalId, logicalId, logicalId, name, QPoint(0, 0));
	}
}

void GraphicalModelView::dataChanged(QModelIndex const &topLeft, QModelIndex const &bottomRight
		, QVector<int> const &roles)
{
	Q_UNUSED(roles)
	for (int row = topLeft.row(); row <= bottomRight.row(); ++row) {
		QModelIndex current = topLeft.sibling(row, 0);

		Id const logicalId = current.data(roles::logicalIdRole).value<Id>();
		static_cast<LogicalModel *>(mModel)->updateElements(logicalId, current.data(Qt::DisplayRole).toString());
	}

	Id const parentLogicalId = topLeft.sibling(topLeft.row(), 0).data(roles::logicalIdRole).value<Id>();
	Id const childLogicalId = bottomRight.sibling(bottomRight.row(), 0).data(roles::logicalIdRole).value<Id>();
	if (((parentLogicalId.editor() == "MetaEditor" && childLogicalId.editor() == "MetaEditor")
			|| (parentLogicalId.editor() == "ConstraintsMetamodel" && childLogicalId.editor() == "ConstraintsMetamodel"))
			&& parentLogicalId != childLogicalId) {
		static_cast<LogicalModel *>(mModel)->changeParent(parentLogicalId, childLogicalId);
	}
}

void GraphicalModelView::rowsAboutToBeRemoved(QModelIndex const &parent, int start, int end)
{
	Q_UNUSED(parent);
	Q_UNUSED(start);
	Q_UNUSED(end);
}
