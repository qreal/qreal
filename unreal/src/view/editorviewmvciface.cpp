#include <QtGui>

#include "editorviewmvciface.h"

#include "editorview.h"
#include "editorviewscene.h"

#include "edge.h"
#include "element.h"

#define _LONG_DEBUG
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
    const Element *e = qgraphicsitem_cast<const Element *>(item);
    if (e)
	emit clicked(e->index());

    if ( const Edge *e = qgraphicsitem_cast<const Edge *>(item) ) {
	emit clicked(e->index());
    }
}

QGraphicsItem * EditorViewMViface::getItem(int uuid)
{
    return items[uuid];
}

void EditorViewMViface::reset()
{ dbg;
	items.clear();
        scene->clearScene();
        rowsInserted(rootIndex(), 0, model()->rowCount(rootIndex()) - 1 );
}

void EditorViewMViface::setModel ( QAbstractItemModel * newModel )
{ dbg;
        QAbstractItemView::setModel(newModel);
        reset();
}

void EditorViewMViface::rowsInserted ( const QModelIndex & parent, int start, int end )
{ dbg;
        for (int row = start; row <= end; ++row) {
            QPersistentModelIndex current = model()->index(row, 0, rootIndex());
            int uuid = model()->index(row, 0, rootIndex()).data().toInt();

            QString type = model()->index(row, 2, rootIndex()).data().toString();
	    
	    qDebug() << "add elem" << uuid << type;

          // FIXME: later
            if (type == "eP2N") {
                        Edge *e = new Edge(this);
                        e->setIndex(current);
                        scene->addItem(e);
                        items[uuid] = e;
            } else {
                        Element *e = new Element(this);
                        e->setIndex(current);
                        scene->addItem(e);
                        items[uuid] = e;
            }
        }

        QAbstractItemView::rowsInserted(parent, start, end);
}

void EditorViewMViface::rowsAboutToBeRemoved ( const QModelIndex & parent, int start, int end )
{ dbg;
        for (int row = start; row <= end; ++row) {
            int uuid = model()->index(row, 0, rootIndex()).data().toInt();

            scene->removeItem(items[uuid]);
            items.remove(uuid);
        }

        QAbstractItemView::rowsAboutToBeRemoved(parent, start, end);
}

