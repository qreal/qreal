#pragma once

#include <QtWidgets/QAbstractItemView>

#include <qrkernel/ids.h>

/// @todo: Make editor view mviface fully private.
#include "qrgui/editor/editorDeclSpec.h"

namespace qReal {

class EditorView;
class EditorViewScene;
class Element;

namespace models {
class Exploser;
class GraphicalModelAssistApi;
class LogicalModelAssistApi;
}

class QRGUI_EDITOR_EXPORT EditorViewMViface : public QAbstractItemView
{
	Q_OBJECT

public:
	EditorViewMViface(qReal::EditorView *view, EditorViewScene *scene);
	~EditorViewMViface();

	QModelIndex indexAt(const QPoint &point) const;
	QRect visualRect(const QModelIndex &index) const;
	void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible);
	bool isDescendentOf(const QModelIndex &descendent, const QModelIndex &ancestor);
	void configure(models::GraphicalModelAssistApi &graphicalAssistApi
			, models::LogicalModelAssistApi &logicalAssistApi
			, models::Exploser &exploser);
	void setLogicalModel(QAbstractItemModel * const logicalModel);
	Id rootId() const;

	EditorViewScene *scene() const;
	models::GraphicalModelAssistApi *graphicalAssistApi() const;
	models::LogicalModelAssistApi *logicalAssistApi() const;

	/// Clears prerendered images.
	/// @param zoomFactor - current zoom factor to render images.
	void invalidateImagesZoomCache(qreal zoomFactor);

signals:
	/// Emitted when for some reason root index was removed from the model.
	void rootElementRemoved(QModelIndex const &graphicsIndex);

public slots:
	void reset();
	void setRootIndex(QModelIndex const &index);
	void rowsAboutToBeMoved(QModelIndex const &sourceParent, int sourceStart, int sourceEnd
			, QModelIndex const &destinationParent, int destinationRow);
	void rowsMoved(QModelIndex const &sourceParent, int sourceStart, int sourceEnd
			, QModelIndex const &destinationParent, int destinationRow);

private slots:
	void dataChanged(QModelIndex const &topLeft, QModelIndex const &bottomRight);
	void rowsAboutToBeRemoved(QModelIndex const &parent, int start, int end);
	void rowsInserted(QModelIndex const &parent, int start, int end);
	void logicalDataChanged(QModelIndex const &topLeft, QModelIndex const &bottomRight);

private:
	typedef QPair<QPersistentModelIndex, Element*> IndexElementPair;

	EditorViewScene *mScene;
	qReal::EditorView *mView;
	models::GraphicalModelAssistApi *mGraphicalAssistApi;
	models::LogicalModelAssistApi *mLogicalAssistApi;
	models::Exploser *mExploser;

	/** @brief elements on the scene. their indices change SUDDENLY, so don't use maps, hashes etc. */
	QSet<IndexElementPair> mItems;

	QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers);

	int horizontalOffset() const;
	int verticalOffset() const;

	bool isIndexHidden(const QModelIndex &index) const;

	void setSelection(const QRect& rect, QItemSelectionModel::SelectionFlags command);

	QRegion visualRegionForSelection(const QItemSelection &selection ) const;

	Element *item(const QPersistentModelIndex &index) const;
	void setItem(const QPersistentModelIndex &index, Element *item);
	void removeItem(const QPersistentModelIndex &index);

	void clearItems();
};

}
