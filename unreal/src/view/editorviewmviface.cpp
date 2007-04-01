#include <QtGui>

#include "editorviewmviface.h"

#include "editorview.h"
#include "editorviewscene.h"

#include "uml_element.h"
#include "uml_guiobjectfactory.h"

EditorViewMViface::EditorViewMViface(EditorView *view, EditorViewScene *scene)
    : QAbstractItemView(0)
{
    this->view = view;
    this->scene = scene;
}

QRect EditorViewMViface::visualRect(const QModelIndex &index) const
{
    return QRect();
}

void EditorViewMViface::scrollTo(const QModelIndex &index, ScrollHint hint)
{
    int uuid = index.sibling(0,index.row()).data().toInt();
}

QModelIndex EditorViewMViface::indexAt(const QPoint &point) const
{
    return QModelIndex();
}

QModelIndex EditorViewMViface::moveCursor(QAbstractItemView::CursorAction cursorAction,
                           Qt::KeyboardModifiers modifiers)
{
    return QModelIndex();
}

int EditorViewMViface::horizontalOffset() const
{
}

int EditorViewMViface::verticalOffset() const
{
}

bool EditorViewMViface::isIndexHidden(const QModelIndex &index) const
{
}

void EditorViewMViface::setSelection(const QRect&, QItemSelectionModel::SelectionFlags command)
{
}

QRegion EditorViewMViface::visualRegionForSelection(const QItemSelection &selection) const
{
}

void EditorViewMViface::raiseClick ( const QGraphicsItem * item )
{
    const UML::Element *e = qgraphicsitem_cast<const UML::Element *>(item);
    if (e)
	emit clicked(e->index());
}

UML::Element* EditorViewMViface::getItem(int uuid)
{
    return items[uuid];
}

void EditorViewMViface::reset()
{
	items.clear();
        scene->clearScene();
        rowsInserted(rootIndex(), 0, model()->rowCount(rootIndex()) - 1 );
}

static void dumpStuff( const QModelIndex & idx )
{
    qDebug() << idx << ":"
	    << idx.sibling(idx.row(),0).data().toString()
	    << idx.sibling(idx.row(),1).data().toString()
	    << idx.sibling(idx.row(),2).data().toString()
	    << idx.sibling(idx.row(),3).data().toString()
	    << idx.sibling(idx.row(),4).data().toString()
	    << idx.sibling(idx.row(),5).data().toString()
	    << idx.sibling(idx.row(),6).data().toString()
	    << idx.sibling(idx.row(),7).data().toString()
	    << idx.sibling(idx.row(),8).data().toString()
	    << idx.sibling(idx.row(),9).data().toString()
	    ;
	    
}

void EditorViewMViface::rowsInserted ( const QModelIndex & parent, int start, int end )
{
	qDebug() << "rowsInserted: adding items";
	for (int row = start; row <= end; ++row) {
            QPersistentModelIndex current = model()->index(row, 0, parent);
//	    dumpStuff(current);
            int uuid = model()->index(row, 0, parent).data().toInt();
            QString type = model()->index(row, 2, parent).data().toString();

	    if ( UML::Element *e = UML::GUIObjectFactory(type) ) {
		scene->addItem(e);
                e->setIndex(current);
	        items[uuid] = e;
	    }
        }
	qDebug() << "rowsInserted: updating items";
        for (int row = start; row <= end; ++row) {
	    int uuid = model()->index(row, 0, parent).data().toInt();
	    if (items.contains(uuid))
		    items[uuid]->updateData();
	}


        QAbstractItemView::rowsInserted(parent, start, end);
}

void EditorViewMViface::rowsAboutToBeRemoved ( const QModelIndex & parent, int start, int end )
{
        //qDebug() << "removed";
        for (int row = start; row <= end; ++row) {
            int uuid = model()->index(row, 0, parent).data().toInt();
	    
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
            int uuid = topLeft.sibling(row, 0).data().toInt();
            items[uuid]->updateData();
        }
}
