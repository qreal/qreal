#pragma once

#include <QtGui/QAbstractItemView>

#include "details/abstractView.h"

namespace qReal {

	namespace models {

		class GraphicalModel;

		class LogicalModelView : public details::AbstractView
		{
			Q_OBJECT
		public:
			LogicalModelView(GraphicalModel * const model);

		protected slots:
			virtual void dataChanged(QModelIndex const &topLeft, QModelIndex const &bottomRight);
		};

	}

}
