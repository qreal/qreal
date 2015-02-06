#pragma once

#include <QtWidgets/QAbstractItemView>

#include "qrgui/models/details/modelsImplementation/abstractView.h"

namespace qReal {
namespace models {
namespace details {

class LogicalModel;

class GraphicalModelView : public modelsImplementation::AbstractView
{
	Q_OBJECT

public:
	GraphicalModelView(LogicalModel * const model);

protected slots:
	virtual void rowsInserted(QModelIndex const &parent, int start, int end);
	virtual void dataChanged(QModelIndex const &topLeft, QModelIndex const &bottomRight
			, QVector<int> const &roles = QVector<int>());
	virtual void rowsAboutToBeRemoved(QModelIndex const &parent, int start, int end);

};

}
}
}
