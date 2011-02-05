#pragma once

#include <QtGui/QAbstractItemView>

#include "modelsImplementation/abstractView.h"

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
	virtual void dataChanged(QModelIndex const &topLeft, QModelIndex const &bottomRight);
	virtual void rowsAboutToBeRemoved(QModelIndex const &parent, int start, int end);

};

}

}

}
