#include <QtGui>

#include "editorviewmvciface.h"

#define _LONG_DEBUG
#include "dbg.h"


EditorViewMVCIface::EditorViewMVCIface(QWidget * parent)
    : QAbstractItemView(parent)
{ dbg;
}

QRect EditorViewMVCIface::visualRect(const QModelIndex &index) const
{ dbg;
    return QRect();
}

void EditorViewMVCIface::scrollTo(const QModelIndex &index, ScrollHint hint)
{ dbg;
}

QModelIndex EditorViewMVCIface::indexAt(const QPoint &point) const
{ dbg;
    return QModelIndex();
}

QModelIndex EditorViewMVCIface::moveCursor(QAbstractItemView::CursorAction cursorAction,
                           Qt::KeyboardModifiers modifiers)
{ dbg;
    return QModelIndex();
}

int EditorViewMVCIface::horizontalOffset() const
{ dbg;
}

int EditorViewMVCIface::verticalOffset() const
{ dbg;
}

bool EditorViewMVCIface::isIndexHidden(const QModelIndex &index) const
{ dbg;
}

void EditorViewMVCIface::setSelection(const QRect&, QItemSelectionModel::SelectionFlags command)
{ dbg;
}

QRegion EditorViewMVCIface::visualRegionForSelection(const QItemSelection &selection) const
{ dbg;
}


void EditorViewMVCIface::reset()
{ dbg;
//        clearScene();
        rowsInserted(rootIndex(), 0, model()->rowCount(rootIndex()) - 1 );
}

void EditorViewMVCIface::rowsInserted ( const QModelIndex & parent, int start, int end )
{ dbg;
        for (int row = start; row <= end; ++row) {
            QPersistentModelIndex current = model()->index(row, 0, rootIndex());
            int uuid = model()->index(row, 0, rootIndex()).data().toInt();

            QString type = model()->index(row, 2, rootIndex()).data().toString();

	    qDebug() << uuid;

/*            // FIXME: later
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
            }*/
        }

        QAbstractItemView::rowsInserted(parent, start, end);
}

void EditorViewMVCIface::rowsAboutToBeRemoved ( const QModelIndex & parent, int start, int end )
{ dbg;
        for (int row = start; row <= end; ++row) {
            int uuid = model()->index(row, 0, rootIndex()).data().toInt();

//            scene->removeItem(items[uuid]);
//            items.remove(uuid);
        }

        QAbstractItemView::rowsAboutToBeRemoved(parent, start, end);
}
