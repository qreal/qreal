#include <QtGui>

#include "editorviewmviface.h"

#include "editorview.h"
#include "editorviewscene.h"

#include "realreporoles.h"

#include "uml_element.h"
#include "uml_guiobjectfactory.h"

EditorViewMViface::EditorViewMViface(EditorView *view, EditorViewScene *scene)
	: QAbstractItemView(0)
{
	this->view = view;
	this->scene = scene;

	//    view->mv_iface = this;
	scene->mv_iface = this;
	scene->view = view;
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
*/

UML::Element* EditorViewMViface::getItem(int uuid)
{
	return items[uuid];
}

void EditorViewMViface::reset()
{
	items.clear();
	scene->clearScene();

	// so that our diagram be nicer	
	scene->removeItem(scene->addRect(QRect(-1000,-1000,2000,2000)));
	
	if ( model() )
		rowsInserted(rootIndex(), 0, model()->rowCount(rootIndex()) - 1 );
}

void EditorViewMViface::setRootIndex(const QModelIndex &index)
{
	QAbstractItemView::setRootIndex(index);
	reset();
}

void EditorViewMViface::rowsInserted ( const QModelIndex & parent, int start, int end )
{
	qDebug() << "========== rowsInserted";

//	if ( parent != rootIndex() )
//		return;
	if ( parent == QModelIndex() )
		return;

//	qDebug() << "rowsInserted: adding items" << parent;
	for (int row = start; row <= end; ++row) {
		QPersistentModelIndex current = model()->index(row, 0, parent);
		int uuid = model()->index(row, 0, parent).data(Unreal::IdRole).toInt();
		int type = model()->index(row, 0, parent).data(Unreal::TypeRole).toInt();

		int parent_uuid;
		if ( parent != rootIndex() )
				parent_uuid = parent.data(Unreal::IdRole).toInt();
		else
				parent_uuid = -1;

		qDebug() << uuid << type;

		if ( ! uuid )
			continue;

		if ( UML::Element *e = UML::GUIObjectFactory(type) ) {
			scene->addItem(e);
			e->setIndex(current);
			e->setPos(current.data(Unreal::PositionRole).toPointF());

			if ( parent_uuid != -1 ) 
				e->setParentItem(items[parent_uuid]);
			
			items[uuid] = e;
		}

		if ( model()->hasChildren(current) ) {
			rowsInserted( current, 0, model()->rowCount( current ) );
		}
	}


//	qDebug() << "rowsInserted: updating items";
	for (int row = start; row <= end; ++row) {
		int uuid = model()->index(row, 0, parent).data(Unreal::IdRole).toInt();
		if (items.contains(uuid))
			items[uuid]->updateData();
	}

	QAbstractItemView::rowsInserted(parent, start, end);
}

void EditorViewMViface::rowsAboutToBeRemoved ( const QModelIndex & parent, int start, int end )
{
	for (int row = start; row <= end; ++row) {
		int uuid = model()->index(row, 0, parent).data(Unreal::IdRole).toInt();

		scene->removeItem(items[uuid]);
		delete items[uuid];
		items.remove(uuid);
	}

	QAbstractItemView::rowsAboutToBeRemoved(parent, start, end);
}

void EditorViewMViface::dataChanged(const QModelIndex &topLeft,
		const QModelIndex &bottomRight)
{
	for (int row = topLeft.row(); row <= bottomRight.row(); ++row) {
		int uuid = topLeft.sibling(row, 0).data(Unreal::IdRole).toInt();

		if ( items.contains(uuid) )
			items[uuid]->updateData();
		else
			rowsInserted(topLeft.parent(),row,row);

	}
}

