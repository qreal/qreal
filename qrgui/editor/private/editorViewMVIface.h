#pragma once

#include <QtWidgets/QAbstractItemView>

#include "models/graphicalModelAssistApi.h"
#include "models/logicalModelAssistApi.h"
#include "plugins/pluginManager/exploser.h"
#include "editor/sdfRenderer.h"

class QGraphicsItem;

namespace qReal {

class EditorViewScene;

class Element;

namespace models {
class GraphicalModelAssistApi;
class LogicalModelAssistApi;
}
class EditorView;

class EditorViewMViface : public QAbstractItemView
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
			, models::LogicalModelAssistApi &logicalAssistApi, Exploser &exploser);
	void setLogicalModel(QAbstractItemModel * const logicalModel);
	Id rootId() const;

	EditorViewScene *scene() const;
	models::GraphicalModelAssistApi *graphicalAssistApi() const;
	models::LogicalModelAssistApi *logicalAssistApi() const;

	/// Clears prerendered images.
	/// @param zoomFactor - current zoom factor to render images.
	void invalidateImagesZoomCache(qreal zoomFactor);

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
	Exploser *mExploser;

	/** @brief elements on the scene. their indices change SUDDENLY, so don't use maps, hashes etc. */
	QSet<IndexElementPair> mItems;

	SdfRenderer mRenderer;

	QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers);

	int horizontalOffset() const;
	int verticalOffset() const;

	bool isIndexHidden(const QModelIndex &index) const;

	void setSelection(const QRect& rect, QItemSelectionModel::SelectionFlags command);

	QRegion visualRegionForSelection(const QItemSelection &selection ) const;

	Element *item(QPersistentModelIndex const &index) const;
	void setItem(QPersistentModelIndex const &index, Element *item);
	void removeItem(QPersistentModelIndex const &index);

	void clearItems();
};

}
