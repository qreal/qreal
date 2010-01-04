/** @file editorviewmviface.cpp
 * 	@brief Класс, реализующий интерфейс представления в схеме Model/View
 * */
#include <QtGui>

#include "editorviewmviface.h"
#include "editorview.h"
#include "editorviewscene.h"
#include "../kernel/definitions.h"
#include "../umllib/uml_element.h"
#include "editormanager.h"
#include "../mainwindow/mainwindow.h"

using namespace qReal;

EditorViewMViface::EditorViewMViface(EditorView *view, EditorViewScene *scene)
	: QAbstractItemView(0)
{
	mView = view;
	mScene = scene;

	mScene->mv_iface = this;
	mScene->view = mView;
}

QRect EditorViewMViface::visualRect(const QModelIndex &) const
{
	return QRect();
}

void EditorViewMViface::scrollTo(const QModelIndex &, ScrollHint)
{
}

QModelIndex EditorViewMViface::indexAt(const QPoint &) const
{
	return QModelIndex();
}

QModelIndex EditorViewMViface::moveCursor(QAbstractItemView::CursorAction,
		Qt::KeyboardModifiers)
{
	return QModelIndex();
}

int EditorViewMViface::horizontalOffset() const
{
	return 0;
}

int EditorViewMViface::verticalOffset() const
{
	return 0;
}

bool EditorViewMViface::isIndexHidden(const QModelIndex &) const
{
	return false;
}

void EditorViewMViface::setSelection(const QRect&, QItemSelectionModel::SelectionFlags )
{
}

QRegion EditorViewMViface::visualRegionForSelection(const QItemSelection &) const
{
	return QRegion();
}

void EditorViewMViface::reset()
{
	mItems.clear();
	mScene->clearScene();
	//для того, чтобы работало с экстерминатусом.
	if ((model()) && (model()->rowCount(QModelIndex()) == 0))
	{
		mScene->setEnabled(false);
	}

	// so that our diagram be nicer
	QGraphicsRectItem *rect = mScene->addRect(QRect(-1000, -1000, 2000, 2000));
	mScene->removeItem(rect);
	delete rect;

	if ( model() )
		rowsInserted(rootIndex(), 0, model()->rowCount(rootIndex()) - 1);
}

void EditorViewMViface::setRootIndex(const QModelIndex &index)
{
	QAbstractItemView::setRootIndex(index);
	reset();
}

void EditorViewMViface::rowsInserted(const QModelIndex &parent, int start, int end)
{
	qDebug() << "========== rowsInserted" << parent << start << end;

	qDebug() << "rowsInserted: adding items" << parent;
	for (int row = start; row <= end; ++row) {
		QPersistentModelIndex current = model()->index(row, 0, parent);
		Id uuid = current.data(roles::idRole).value<Id>();

		Id parent_uuid;
		if (parent != rootIndex())
			parent_uuid = parent.data(roles::idRole).value<Id>();

		mScene->setEnabled(true);

		//если добавляем диаграмму в корень
		if (!parent.isValid())
		{
			setRootIndex(current);
			continue;
		}

		qDebug() << uuid.toString();

		if (uuid == ROOT_ID)
			continue;

		UML::Element *e = mScene->mainWindow()->manager()->graphicalObject(uuid);
		if (e) {
			e->setIndex(current);
			if ((!(parent_uuid == Id())) && (mItems.contains(parent)))
				e->setParentItem(mItems[parent]);
			else
				mScene->addItem(e);
			mItems[current] = e;
			e->updateData();
			e->connectToPort();
		}

		if (model()->hasChildren(current)) {
			rowsInserted(current, 0, model()->rowCount(current) - 1);
		}
	}
	QAbstractItemView::rowsInserted(parent, start, end);
}

void EditorViewMViface::rowsAboutToBeRemoved ( const QModelIndex & parent, int start, int end )
{
	for (int row = start; row <= end; ++row) {
		QModelIndex curr = model()->index(row, 0, parent);
		mScene->removeItem(mItems[curr]);
		delete mItems[curr];
		mItems.remove(curr);
	}
	//потому что из модели элементы удаляются только после того, как удалятся из графической части.
	if ((parent == QModelIndex()) && (model()->rowCount(parent) == start - end + 1))
	{
		mScene->setEnabled(false);
	}
	QAbstractItemView::rowsAboutToBeRemoved(parent, start, end);
}

void EditorViewMViface::rowsAboutToBeMoved(QModelIndex const &sourceParent, int sourceStart, int sourceEnd, QModelIndex const &destinationParent, int destinationRow)
{
	Q_ASSERT(sourceStart == sourceEnd);  // Можно перемещать только один элемент за раз.
	QModelIndex movedElementIndex = sourceParent.child(sourceStart, 0);
	UML::Element* movedElement = mItems[movedElementIndex];
	UML::Element* newParent = mItems[destinationParent];
	movedElement->setParentItem(newParent);
}

void EditorViewMViface::rowsMoved(QModelIndex const &sourceParent, int sourceStart, int sourceEnd, QModelIndex const &destinationParent, int destinationRow)
{
	Q_ASSERT(sourceStart == sourceEnd);
	QPersistentModelIndex movedElementIndex = destinationParent.child(destinationRow, 0);
	UML::Element* movedElement = mItems[movedElementIndex];
	movedElement->updateData();
}

void EditorViewMViface::dataChanged(const QModelIndex &topLeft,
	const QModelIndex &bottomRight)
{
	for (int row = topLeft.row(); row <= bottomRight.row(); ++row) {
		QModelIndex curr = topLeft.sibling(row, 0);
		if (mItems.contains(curr)) {
			Q_ASSERT(mItems[curr] != NULL);
			mItems[curr]->updateData();
		}
	}
}

EditorViewScene *EditorViewMViface::scene() const
{
	return mScene;
}
