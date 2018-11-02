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

#include <qrkernel/ids.h>

#include "editor/edgeElement.h"
#include "editor/nodeElement.h"

/// @todo: Make editor view mviface fully private.
#include "qrgui/editor/editorDeclSpec.h"

namespace qReal {

namespace models {
class Exploser;
class GraphicalModelAssistApi;
class LogicalModelAssistApi;
}


namespace gui {
namespace editor {

class EditorView;
class EditorViewScene;
class Element;

class QRGUI_EDITOR_EXPORT EditorViewMViface : public QAbstractItemView
{
	Q_OBJECT

public:
	EditorViewMViface(EditorView *view, EditorViewScene *scene);
	~EditorViewMViface() override;

	QModelIndex indexAt(const QPoint &point) const override;
	QRect visualRect(const QModelIndex &index) const override;
	void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible) override;
	bool isDescendentOf(const QModelIndex &descendent, const QModelIndex &ancestor);
	void configure(models::GraphicalModelAssistApi &graphicalAssistApi
			, models::LogicalModelAssistApi &logicalAssistApi
			, models::Exploser &exploser);
	void setLogicalModel(QAbstractItemModel * const logicalModel);
	Id rootId() const;

	EditorViewScene *scene() const;
	models::GraphicalModelAssistApi *graphicalAssistApi() const;
	models::LogicalModelAssistApi *logicalAssistApi() const;

signals:
	/// Emitted when for some reason root index was removed from the model.
	void rootElementRemoved(const QModelIndex &graphicsIndex);

public slots:
	void reset() override;
	void setRootIndex(const QModelIndex &index) override;
	void rowsAboutToBeMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd
			, const QModelIndex &destinationParent, int destinationRow);
	void rowsMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd
			, const QModelIndex &destinationParent, int destinationRow);

private slots:
	void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight
			, const QVector<int> &roles = QVector<int>()) override;
	void rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end) override;
	void rowsInserted(const QModelIndex &parent, int start, int end) override;
	void logicalDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

private:
	typedef QPair<QPersistentModelIndex, Element*> IndexElementPair;

	EditorViewScene *mScene;
	EditorView *mView;
	models::GraphicalModelAssistApi *mGraphicalAssistApi;
	models::LogicalModelAssistApi *mLogicalAssistApi;
	models::Exploser *mExploser;

	/** @brief elements on the scene. their indices change SUDDENLY, so don't use maps, hashes etc. */
	QSet<IndexElementPair> mItems;

	QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers) override;

	int horizontalOffset() const override;
	int verticalOffset() const override;

	bool isIndexHidden(const QModelIndex &index) const override;

	void setSelection(const QRect& rect, QItemSelectionModel::SelectionFlags command) override;

	QRegion visualRegionForSelection(const QItemSelection &selection ) const override;

	Element *item(const QPersistentModelIndex &index) const;
	void setItem(const QPersistentModelIndex &index, Element *item);
	void removeItem(const QPersistentModelIndex &index);
	void clearItems();

	void handleAddingSequenceForRowsInserted(const QModelIndex &parent
		, Element *elem, const QPersistentModelIndex &current);

	void handleElemDataForRowsInserted(Element *elem, const QPersistentModelIndex &current);
	void handleNodeElementsForRowsInserted(const QList<QPair<NodeElement *, QPersistentModelIndex>> &nodes
			, const QModelIndex &parent);
	void handleEdgeElementsForRowsInserted(const QList<QPair<EdgeElement *, QPersistentModelIndex>> &edges
			, const QModelIndex &parent);
};

}
}
}
