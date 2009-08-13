/** @file editorviewmviface.cpp
 * 	@brief Класс, реализующий интерфейс представления в схеме Model/View
 * */
#include <QtGui>

#include "editorviewmviface.h"

#include "editorview.h"
#include "editorviewscene.h"

#include "../kernel/definitions.h"
//#include "realreporoles.h"

#include "../umllib/uml_element.h"
//#include "uml_guiobjectfactory.h"


#include "editormanager.h"
#include "../mainwindow/mainwindow.h"

using namespace qReal;

EditorViewMViface::EditorViewMViface(EditorView *view, EditorViewScene *scene)
	: QAbstractItemView(0)
{
	this->view = view;
	this->mScene = scene;

	//    view->mv_iface = this;
	mScene ->mv_iface = this;
	mScene ->view = view;
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
/*
void EditorViewMViface::raiseClick ( const QGraphicsItem * item )
{
	const UML::Element *e = qgraphicsitem_cast<const UML::Element *>(item);
	if (e)
		emit clicked(e->index());
}

UML::Element* EditorViewMViface::getItem(IdType const &uuid)
{
	return items[uuid];
}
*/

void EditorViewMViface::reset()
{
	items.clear();
	mScene->clearScene();

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

//	if (parent == QModelIndex() || parent.parent() == QModelIndex())
//		return;

	qDebug() << "rowsInserted: adding items" << parent;
	for (int row = start; row <= end; ++row) {
		QPersistentModelIndex current = model()->index(row, 0, parent);
		IdType uuid = current.data(roles::idRole).value<Id>();
	//	TypeIdType type = current.data(Unreal::TypeRole).toString();

		IdType parent_uuid;
		if (parent != rootIndex())
			parent_uuid = parent.data(roles::idRole).value<Id>();

		qDebug() << uuid.toString();

		if (uuid == ROOT_ID)
			continue;

		//if (UML::Element *e = UML::GUIObjectFactory(type)) {
		UML::Element *e = mScene->mainWindow()->manager()->graphicalObject(uuid);
		if (e) {
			e->setIndex(current);
			if (!(parent_uuid == Id()))
				e->setParentItem(items[parent]);
			else
				mScene->addItem(e);
			items[current] = e;
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
		mScene->removeItem(items[curr]);
		delete items[curr];
		items.remove(curr);
	}

	QAbstractItemView::rowsAboutToBeRemoved(parent, start, end);
}

void EditorViewMViface::dataChanged(const QModelIndex &topLeft,
	const QModelIndex &bottomRight)
{
	for (int row = topLeft.row(); row <= bottomRight.row(); ++row) {
		QModelIndex curr = topLeft.sibling(row, 0);
		if (items.contains(curr)) {
			Q_ASSERT(items[curr] != NULL);
			items[curr]->updateData();
		}
		else
			rowsInserted(topLeft.parent(),row,row);
	}
}

EditorViewScene *EditorViewMViface::scene() const
{
	return mScene;
}
