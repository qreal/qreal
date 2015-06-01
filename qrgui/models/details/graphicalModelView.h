/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
	virtual void rowsInserted(const QModelIndex &parent, int start, int end);
	virtual void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight
			, QVector<int> const &roles = QVector<int>());
	virtual void rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);

};

}
}
}
