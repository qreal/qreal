#pragma once

#include <QAbstractItemView>
#include "../models/graphicalModelAssistApi.h"
#include "../models/logicalModelAssistApi.h"

class QGraphicsItem;

class EditorViewScene;

namespace UML {
	class Element;
}

namespace qReal {

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
		void setAssistApi(models::GraphicalModelAssistApi &graphicalAssistApi, models::LogicalModelAssistApi &logicalAssistApi);

		EditorViewScene *scene() const;

	public slots:
		void reset();
		void setRootIndex(const QModelIndex &index);
		void rowsAboutToBeMoved(QModelIndex const &sourceParent, int sourceStart, int sourceEnd, QModelIndex const &destinationParent, int destinationRow);
		void rowsMoved(QModelIndex const &sourceParent, int sourceStart, int sourceEnd, QModelIndex const &destinationParent, int destinationRow);

	protected slots:
		void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
		void rowsAboutToBeRemoved ( const QModelIndex & parent, int start, int end);
		void rowsInserted ( const QModelIndex & parent,	int start, int end);

	protected:
		QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers);

		int horizontalOffset() const;
		int verticalOffset() const;

		bool isIndexHidden(const QModelIndex &index) const;

		void setSelection(const QRect& rect, QItemSelectionModel::SelectionFlags command);

		QRegion visualRegionForSelection(const QItemSelection &selection ) const;

	private:
		typedef QPair<QPersistentModelIndex, UML::Element*> IndexElementPair;

		EditorViewScene *mScene;
		qReal::EditorView *mView;
		models::GraphicalModelAssistApi *mGraphicalAssistApi;
		models::LogicalModelAssistApi *mLogicalAssistApi;


		/** @brief elements on the scene. their indices change SUDDENLY, so don't use maps, hashes etc. */
		QSet<IndexElementPair> mItems;

		UML::Element *item(QPersistentModelIndex const &index) const;
		void setItem(QPersistentModelIndex const &index, UML::Element *item);
		void removeItem(QPersistentModelIndex const &index);

		void clearItems();
	};

}
