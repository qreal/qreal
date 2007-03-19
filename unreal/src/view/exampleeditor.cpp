#include <QtGui>

#include <iostream>
#include <typeinfo>

#include "exampleeditor.h"
//#define _LONG_DEBUG
#include "dbg.h"

//#include "uml_comment.h"

#include "treeitem.h"
#include "element.h"
#include "edge.h"

#include "diagramexplorermodel.h"

#include <stdio.h>

ExampleEditor::ExampleEditor(QWidget *parent)
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

void ExampleEditor::dataChanged(const QModelIndex &topLeft,
		const QModelIndex &bottomRight)
{ dbg;
}

void ExampleEditor::selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected )
{ dbg;

	QList<QModelIndex> list;
	QList<QModelIndex>::Iterator it;

	list = deselected.indexes();
	//    for (it = list.begin(); it != list.end(); ++it) {
	//        items[(*it).internalId()]->setSelected(false);
	//    }

	list = selected.indexes();
	//    for (it = list.begin(); it != list.end(); ++it) {
	//        items[(*it).internalId()]->setSelected(true);
	//    }

}

bool ExampleEditor::edit(const QModelIndex &index, EditTrigger trigger, QEvent *event)
{ dbg;
	/*    if (index.column() == 0)
	      return QAbstractItemView::edit(index, trigger, event);
	      else
	      return false; */
     return false;     
}

QModelIndex ExampleEditor::indexAt(const QPoint &point) const
{ dbg;
	return QModelIndex();
}

bool ExampleEditor::isIndexHidden(const QModelIndex & /*index*/) const
{ dbg;
	return false;
}

QModelIndex ExampleEditor::moveCursor(QAbstractItemView::CursorAction cursorAction,
		Qt::KeyboardModifiers /*modifiers*/)
{ dbg;
	return currentIndex();
}

int ExampleEditor::rows(const QModelIndex &index) const
{ dbg;
	return model()->rowCount(model()->parent(index));
}

void ExampleEditor::rowsInserted(const QModelIndex &parent, int start, int end)
{ dbg;
	
}

void ExampleEditor::setModel ( QAbstractItemModel * newModel )
{ dbg;
	QAbstractItemView::setModel(newModel);
        clearScene();
	items.clear();
	reset();
}

void ExampleEditor::clearScene ()
{ dbg;
	QList<QGraphicsItem *> list = scene->items();
	QList<QGraphicsItem *>::Iterator it = list.begin();
	for (; it != list.end(); ++it) {
		if ( *it )
			delete *it;
	}

}

void ExampleEditor::rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end)
{ dbg;
	QAbstractItemView::rowsAboutToBeRemoved(parent, start, end);
}

void ExampleEditor::scrollTo(const QModelIndex &index, ScrollHint)
{ dbg;
	//    view->ensureVisible(items[index.internalId()]);
}

void ExampleEditor::setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command)
{ dbg;
}

void ExampleEditor::reset()
{ dbg;
	clearScene();

	for (int row = 0; row < model()->rowCount(rootIndex()); ++row) {
	    QPersistentModelIndex current = model()->index(row, 0, rootIndex());
	    
	    Element *e = new Element();
	    e->setIndex(current);
	    scene->addItem(e);
	    
	    qDebug() << "creating an index for element unknown";
	}
}

QRect ExampleEditor::visualRect(const QModelIndex &index) const
{
	return QRect();
}

QRegion ExampleEditor::visualRegionForSelection(const QItemSelection &selection) const
{
	return QRegion();
}

int ExampleEditor::horizontalOffset() const
{
	return 0;
}

int ExampleEditor::verticalOffset() const
{
	return 0;
}
