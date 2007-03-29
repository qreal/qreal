#include <QtGui>

#include "editorviewmviface.h"

#include "editorview.h"
#include "editorviewscene.h"

#include "edge.h"
#include "element.h"

#include "uml_edgeelement.h"
#include "uml_nodeelement.h"

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

void EditorViewMViface::rowsInserted ( const QModelIndex & parent, int start, int end )
{ dbg;
	qDebug() << "rowsInserted";
        for (int row = start; row <= end; ++row) {
            QPersistentModelIndex current = model()->index(row, 0, parent);
            int uuid = model()->index(row, 0, parent).data().toInt();

            QString type = model()->index(row, 2, parent).data().toString();

	    qDebug() << "row" << row << "uuid" << uuid << "type" << type;
	    
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

        QAbstractItemView::rowsInserted(parent, start, end);
}

void EditorViewMViface::rowsAboutToBeRemoved ( const QModelIndex & parent, int start, int end )
{ dbg;
        qDebug() << "removed";
        for (int row = start; row <= end; ++row) {
            int uuid = model()->index(row, 0, parent).data().toInt();
	    
            scene->removeItem(items[uuid]);
            items.remove(uuid);
        }

        QAbstractItemView::rowsAboutToBeRemoved(parent, start, end);
}

void EditorViewMViface::dataChanged(const QModelIndex &topLeft,
                const QModelIndex &bottomRight)
{ dbg;

        for (int row = topLeft.row(); row <= bottomRight.row(); ++row) {
            int uuid = topLeft.sibling(row, 0).data().toInt();

            QPersistentModelIndex current = topLeft.sibling(row, 0);
            qgraphicsitem_cast<UML::Element *>(items[uuid])->setIndex(current);
        }
}
