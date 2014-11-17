#pragma once

#include <QtWidgets/QAbstractItemView>

#include "qrgui/models/details/modelsImplementation/abstractView.h"

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
	virtual void dataChanged(QModelIndex const &topLeft, QModelIndex const &bottomRight
			, QVector<int> const &roles = QVector<int>());
	virtual void rowsAboutToBeRemoved(QModelIndex const &parent, int start, int end);
};

}
}
}
