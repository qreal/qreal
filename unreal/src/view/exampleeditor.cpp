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
	    int uuid = model()->index(row, 0, rootIndex()).data().toInt();
	    	    
	    Element *e = new Element();
	    e->setIndex(current);
	    scene->addItem(e);
	    
	    items[uuid] = e;
	    
	    qDebug() << "creating an index for element unknown";
	}
}

void ExampleEditor::dumpStuff( const QModelIndex & idx)
{ dbg;
	std::cerr << "--- dumpStuff ---";
        for (int row = 0; row < model()->rowCount(idx); ++row) {
            std::cerr << "dumpStuff: " << row << " - ";
	    for (int col = 0; col <= 10; ++col )
		std::cerr << model()->index(row, col, idx).data().toString().toStdString() << " ";
	    std::cerr << std::endl;
        }
}

void ExampleEditor::rowsInserted ( const QModelIndex & parent, int start, int end )
{ dbg;
	qDebug() << "rowsInserted " << start << end;
	
	dumpStuff(parent);

        for (int row = start; row <= end; ++row) {
            int uuid = model()->index(row, 0, rootIndex()).data().toInt();

//	    int uuid = parent.data().toInt();

	    qDebug() << "adding elements with uuid" << uuid;
	}								

        QAbstractItemView::rowsInserted(parent, start, end);
}

void ExampleEditor::rowsAboutToBeRemoved ( const QModelIndex & parent, int start, int end )
{ dbg;
        for (int row = start; row <= end; ++row) {
	    int uuid = model()->index(row, 0, rootIndex()).data().toInt();
	    
	    qDebug() << "removing elements with uuid" << uuid;
	    
	    scene->removeItem(items[uuid]);
	    items.remove(uuid);
	}	

        QAbstractItemView::rowsAboutToBeRemoved(parent, start, end);
}

void ExampleEditor::dataChanged(const QModelIndex &topLeft,
		const QModelIndex &bottomRight)
{ dbg;
	qDebug() << "dataChanged!!!! dataChanged!!!";
	
	dumpStuff(rootIndex());

	for (int row = topLeft.row(); row <= bottomRight.row(); ++row) {
	    int uuid = model()->index(row, 0, rootIndex()).data().toInt();
	    
	    QPersistentModelIndex current = model()->index(row, 0, rootIndex());
	                
	    qgraphicsitem_cast<Element *>(items[uuid])->setIndex(current);
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
