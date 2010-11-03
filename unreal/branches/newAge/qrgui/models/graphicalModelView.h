#pragma once

#include <QtGui/QAbstractItemView>

#include "details/abstractView.h"

namespace qReal {

	namespace models {

		class LogicalModel;

		class GraphicalModelView : public details::AbstractView
		{
			Q_OBJECT
		public:
			GraphicalModelView(LogicalModel * const model);
		protected slots:
			virtual void rowsInserted(QModelIndex const &parent, int start, int end);
			virtual void dataChanged(QModelIndex const &topLeft, QModelIndex const &bottomRight);

		};

	}

}
