#include <QtGui>

#include <iostream>

#include "pieview.h"
#include "dbg.h"

#include "uml_comment.h"
#include "element.h"
#include "edge.h"

PieView::PieView(QWidget *parent)
{ dbg;
    scene = new QGraphicsScene(-400,-300,800,600);
    view = new QGraphicsView(scene,this);
    view->setRenderHint(QPainter::Antialiasing);

    viewport()->hide();

    setFrameStyle(QFrame::NoFrame);   
    QHBoxLayout *l = new QHBoxLayout(this);
    l->setMargin(0);
    l->setSpacing(0);
    l->addWidget(view);
    setLayout(l); 
}

void PieView::dataChanged(const QModelIndex &topLeft,
                          const QModelIndex &bottomRight)
{ dbg;
    for (int row = topLeft.row(); row <= bottomRight.row(); ++row) {
        QModelIndex typeIndex = model()->index(row, 0, rootIndex());
        QString type = model()->data(typeIndex).toString();

        QModelIndex nameIndex = model()->index(row, 1, rootIndex());
        QString name = model()->data(nameIndex).toString();
	
	if (Element *foo = qgraphicsitem_cast<Element *>(items[ model()->index(row, 0, rootIndex()).internalId() ]))
	    foo->setInfo(type, name);
    }
}

void PieView::selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected )
{ dbg;

    QList<QModelIndex> list;
    QList<QModelIndex>::Iterator it;

    list = deselected.indexes();
    for (it = list.begin(); it != list.end(); ++it) {
        items[(*it).internalId()]->setSelected(false);
    }

    list = selected.indexes();
    for (it = list.begin(); it != list.end(); ++it) {
        items[(*it).internalId()]->setSelected(true);
    }
    
}

bool PieView::edit(const QModelIndex &index, EditTrigger trigger, QEvent *event)
{ dbg;
/*    if (index.column() == 0)
        return QAbstractItemView::edit(index, trigger, event);
    else
        return false; */
}

QModelIndex PieView::indexAt(const QPoint &point) const
{ dbg;
    return QModelIndex();
}

bool PieView::isIndexHidden(const QModelIndex & /*index*/) const
{ dbg;
    return false;
}

QModelIndex PieView::moveCursor(QAbstractItemView::CursorAction cursorAction,
                                Qt::KeyboardModifiers /*modifiers*/)
{ dbg;
    return currentIndex();
}

int PieView::rows(const QModelIndex &index) const
{ dbg;
    return model()->rowCount(model()->parent(index));
}

void PieView::rowsInserted(const QModelIndex &parent, int start, int end)
{ dbg;
    clearScene();

    for (int row = start; row <= end; ++row) {
        QModelIndex typeIndex = model()->index(row, 0, rootIndex());
        QString type = model()->data(typeIndex).toString();

        QModelIndex nameIndex = model()->index(row, 1, rootIndex());
        QString name = model()->data(nameIndex).toString();
	
	Element *foo = new Comment;
	items[model()->index(row, 0, rootIndex()).internalId()] = foo;
	foo->setInfo(type, name);
	scene->addItem(foo);
    }

}

void PieView::setModel ( QAbstractItemModel * newModel )
{ dbg;
    QAbstractItemView::setModel(newModel);
    
    clearScene();
    
    Element *last, *prelast;
    
    for (int row = 0; row < model()->rowCount(rootIndex()); ++row) {
	QModelIndex typeIndex = model()->index(row, 0, rootIndex());
	QString type = model()->data(typeIndex).toString();
	
	QModelIndex nameIndex = model()->index(row, 1, rootIndex());
	QString name = model()->data(nameIndex).toString();
	
	
	std::cout << type.toLocal8Bit().constData() << std::endl;
	
	if ( type != "link" ) {
	    prelast = last;
	    last = new Comment;
	    items[model()->index(row, 0, rootIndex()).internalId()] = last;
	    last->setInfo(type, name);
	    scene->addItem(last);
	} else {
	    Edge *edge = new Edge;
	    
//	    edge->setSource(qgraphicsitem_cast<Element *>(items[i.key()]));
//	    edge->setDest  (qgraphicsitem_cast<Element *>(items[(++i).key()]));
	    edge->setSource(prelast);
	    edge->setDest(last);
	    
	    items[model()->index(row, 0, rootIndex()).internalId()] = edge;
	    scene->addItem(edge);
	}
    }
    
    
}

void PieView::clearScene ()
{
    QList<QGraphicsItem *> list = scene->items();
    QList<QGraphicsItem *>::Iterator it = list.begin();
    for (; it != list.end(); ++it) {
        if ( *it )
            delete *it;
    }
					
}

void PieView::rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end)
{ dbg;


    QAbstractItemView::rowsAboutToBeRemoved(parent, start, end);
}

void PieView::scrollTo(const QModelIndex &index, ScrollHint)
{ dbg;
    view->ensureVisible(items[index.internalId()]);
}

void PieView::setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command)
{ dbg;
}

QRect PieView::visualRect(const QModelIndex &index) const
{
    return QRect();
}

QRegion PieView::visualRegionForSelection(const QItemSelection &selection) const
{
    return QRegion();
}

int PieView::horizontalOffset() const
{
    return 0;
}

int PieView::verticalOffset() const
{
    return 0;
}
