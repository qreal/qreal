#include <QtGui>

#include "editorviewmviface.h"

#include "editorview.h"
#include "editorviewscene.h"

#include "uml_edgeelement.h"
#include "uml_nodeelement.h"

//#define _LONG_DEBUG
#include "dbg.h"

EditorViewMViface::EditorViewMViface(EditorView *view, EditorViewScene *scene)
    : QAbstractItemView(0)
{ dbg;
    this->view = view;
    this->scene = scene;
}

QRect EditorViewMViface::visualRect(const QModelIndex &index) const
{ dbg;
    return QRect();
}

void EditorViewMViface::scrollTo(const QModelIndex &index, ScrollHint hint)
{ dbg;
    int uuid = index.sibling(0,index.row()).data().toInt();
    qDebug() << "scroll to" << uuid;
}

QModelIndex EditorViewMViface::indexAt(const QPoint &point) const
{ dbg;
    return QModelIndex();
}

QModelIndex EditorViewMViface::moveCursor(QAbstractItemView::CursorAction cursorAction,
                           Qt::KeyboardModifiers modifiers)
{ dbg;
    return QModelIndex();
}

int EditorViewMViface::horizontalOffset() const
{ dbg;
}

int EditorViewMViface::verticalOffset() const
{ dbg;
}

bool EditorViewMViface::isIndexHidden(const QModelIndex &index) const
{ dbg;
}

void EditorViewMViface::setSelection(const QRect&, QItemSelectionModel::SelectionFlags command)
{ dbg;
}

QRegion EditorViewMViface::visualRegionForSelection(const QItemSelection &selection) const
{ dbg;
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
{ dbg;
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
	    << idx.sibling(idx.row(),7).data().toString();
}

void EditorViewMViface::rowsInserted ( const QModelIndex & parent, int start, int end )
{ dbg;
	qDebug() << "rowsInserted" << parent;
        for (int row = start; row <= end; ++row) {
            QPersistentModelIndex current = model()->index(row, 0, parent);
	    dumpStuff(current);
            int uuid = model()->index(row, 0, parent).data().toInt();
            QString type = model()->index(row, 2, parent).data().toString();

	    UML::Element *e;

            if (type == "eP2N") {
			e = new UML::EdgeElement();
	    } else {
			e = new UML::NodeElement();
	    }
            scene->addItem(e);
            e->setIndex(current);
            items[uuid] = e;
        }
        for (int row = start; row <= end; ++row) {
	    int uuid = model()->index(row, 0, parent).data().toInt();
	    items[uuid]->updateData();
	}


        QAbstractItemView::rowsInserted(parent, start, end);
}

void EditorViewMViface::rowsAboutToBeRemoved ( const QModelIndex & parent, int start, int end )
{ dbg;
        qDebug() << "removed";
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
{ dbg;

        for (int row = topLeft.row(); row <= bottomRight.row(); ++row) {
            int uuid = topLeft.sibling(row, 0).data().toInt();
            items[uuid]->updateData();
        }
}
