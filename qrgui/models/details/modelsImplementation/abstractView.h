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

#include "qrgui/models/details/modelsImplementation/abstractModel.h"

namespace qReal {
namespace models {
namespace details {
namespace modelsImplementation {

class AbstractView : public QAbstractItemView
{
	Q_OBJECT

public:
	explicit AbstractView(AbstractModel * const model);
	virtual ~AbstractView();

public slots:
	void rowsAboutToBeMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd
			, const QModelIndex &destinationParent, int destinationRow);
	void rowsMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd
			, const QModelIndex &destinationParent, int destinationRow);

protected slots:
	virtual void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight
			, QVector<int> const &roles = QVector<int>());
	virtual void rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);
	virtual void rowsInserted(const QModelIndex &parent, int start, int end);

protected:
	AbstractModel * const mModel;

	// Unneeded stuff from QAbstractItemView. It would be helpful if Qt Model/View framework
	// were moved to QtCore and got separated from GUI concerns, since Model/View pattern
	// is more general.
	QModelIndex indexAt(const QPoint &point) const;
	QRect visualRect(const QModelIndex &index) const;
	void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible);
	bool isDescendentOf(const QModelIndex &descendent, const QModelIndex &ancestor);
	QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers);
	int horizontalOffset() const;
	int verticalOffset() const;
	bool isIndexHidden(const QModelIndex &index) const;
	void setSelection(const QRect& rect, QItemSelectionModel::SelectionFlags command);
	QRegion visualRegionForSelection(const QItemSelection &selection ) const;
};

}
}
}
}
