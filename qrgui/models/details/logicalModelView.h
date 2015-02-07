#pragma once

#include <QtWidgets/QAbstractItemView>

#include "models/details/modelsImplementation/abstractView.h"

namespace qReal {
namespace models {
namespace details {

class GraphicalModel;

class LogicalModelView : public modelsImplementation::AbstractView
{
	Q_OBJECT

public:
	explicit LogicalModelView(GraphicalModel * const model);

protected slots:
	virtual void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight
			, QVector<int> const &roles = QVector<int>());
	virtual void rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);
};

}
}
}
