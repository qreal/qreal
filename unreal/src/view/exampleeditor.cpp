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
	for (int row = start; row <= end; ++row) {
	}

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
	//	QAbstractItemView::reset();

	//	clearScene();
	//	items.clear();

	QMap<QString,int> seen;
	seen.clear();
	for (int row = 0; row < model()->rowCount(rootIndex()); ++row) {
		 
		TreeItem * ti = static_cast<TreeItem *>( (model()->index(row, 0, rootIndex())).internalPointer() );
		QString name = ti->getName();
		QString type = ti->getDiagramName();
		QString idx =  QString("%1").arg(ti->getID());

		seen[idx] = 666;
		
		if ( items.contains(idx) )
			continue;
		
		if ( ti->getType() == "eP2N" )
			continue;	// another kludge, we handle these later...
			
		Element *last = new Element(ti->getType());
		items[idx] = last;
		last->setInfo(ti->getType(), ti->getName());
		scene->addItem(last);
	}
	
	for (int row = 0; row < model()->rowCount(rootIndex()); ++row) {
		TreeItem * ti = static_cast<TreeItem *>( (model()->index(row, 0, rootIndex())).internalPointer() );
		QString name = ti->getName();
		QString type = ti->getDiagramName();
		QString idx =  QString("%1").arg(ti->getID());
		if ( ti->getType() != "eP2N" )
		    continue;

    	Edge *foo = new Edge;
		items[idx] = foo;
	
		foo->setText(type + "/" + name);

		QModelIndex current = model()->index(row, 0, rootIndex());

		DiagramExplorerModel *demodel = dynamic_cast<DiagramExplorerModel *> (model());

		QModelIndex linkSource = demodel->getBeginning( current );
		QModelIndex linkDest = demodel->getEnding( current );
		TreeItem *tiSource = static_cast<TreeItem *>( linkSource.internalPointer() );
		TreeItem *tiDest = static_cast<TreeItem *>( linkDest.internalPointer() );
		QString idxSource =  QString("%1").arg(tiSource->getID());
		QString idxDest =  QString("%1").arg(tiDest->getID());

		foo->setSource(static_cast<Element *> (items[idxSource]));
		foo->setDest(static_cast<Element *> (items[idxDest]));
	
//		for( QMap<QString, QGraphicsItem *>::iterator i = items.begin(); i != items.end(); i++ ) {
//			qDebug() << i.key();
//		}
		
		scene->addItem(foo);
	}
    			

	// Remove items not seen this time

	for( QMap<QString, QGraphicsItem *>::iterator i = items.begin(); i != items.end();  ) {
		if ( seen[i.key()] == 666 ) {
			i++;
		} else {
			scene->removeItem(i.value());
			delete i.value();
			i = items.erase(i);
		}
	}

	scene->update(view->sceneRect());
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
