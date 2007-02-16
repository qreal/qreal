#include <QtGui>

#include <iostream>

#include "pieview.h"
//#define _LONG_DEBUG
#include "dbg.h"

//#include "uml_comment.h"

#include "treeitem.h"
#include "element.h"
#include "edge.h"

#include <stdio.h>

PieView::PieView(QWidget *parent)
{ dbg;
qDebug() << __FUNCTION__;
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
qDebug() << __FUNCTION__;
}

void PieView::selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected )
{ dbg;
qDebug() << __FUNCTION__;

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

bool PieView::edit(const QModelIndex &index, EditTrigger trigger, QEvent *event)
{ dbg;
qDebug() << __FUNCTION__;
	/*    if (index.column() == 0)
	      return QAbstractItemView::edit(index, trigger, event);
	      else
	      return false; */
}

QModelIndex PieView::indexAt(const QPoint &point) const
{ dbg;
qDebug() << __FUNCTION__;
	return QModelIndex();
}

bool PieView::isIndexHidden(const QModelIndex & /*index*/) const
{ dbg;
qDebug() << __FUNCTION__;
	return false;
}

QModelIndex PieView::moveCursor(QAbstractItemView::CursorAction cursorAction,
		Qt::KeyboardModifiers /*modifiers*/)
{ dbg;
qDebug() << __FUNCTION__;
	return currentIndex();
}

int PieView::rows(const QModelIndex &index) const
{ dbg;
qDebug() << __FUNCTION__;
	return model()->rowCount(model()->parent(index));
}

void PieView::rowsInserted(const QModelIndex &parent, int start, int end)
{ dbg;
qDebug() << __FUNCTION__;
//	clearScene();

//	for (int row = start; row <= end; ++row) {
//		QModelIndex typeIndex = model()->index(row, 0, rootIndex());
//		QString type = model()->data(typeIndex).toString();

//		QModelIndex nameIndex = model()->index(row, 1, rootIndex());
//		QString name = model()->data(nameIndex).toString();

		/*	Element *foo = new Comment;
			items[model()->index(row, 0, rootIndex()).internalId()] = foo;
			foo->setInfo(type, name);
			scene->addItem(foo);*/
//	}

}

void PieView::setModel ( QAbstractItemModel * newModel )
{ dbg;
qDebug() << __FUNCTION__;
	QAbstractItemView::setModel(newModel);
	reset();
}

void PieView::clearScene ()
{
qDebug() << __FUNCTION__;
	QList<QGraphicsItem *> list = scene->items();
	QList<QGraphicsItem *>::Iterator it = list.begin();
	for (; it != list.end(); ++it) {
		if ( *it )
			delete *it;
	}

}

void PieView::rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end)
{ dbg;

qDebug() << __FUNCTION__;
	QAbstractItemView::rowsAboutToBeRemoved(parent, start, end);
}

void PieView::scrollTo(const QModelIndex &index, ScrollHint)
{ dbg;
qDebug() << __FUNCTION__;
	//    view->ensureVisible(items[index.internalId()]);
}

void PieView::setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command)
{ dbg;
qDebug() << __FUNCTION__;
}

void PieView::reset()
{ dbg;
qDebug() << __FUNCTION__;
	//	QAbstractItemView::reset();

	//	clearScene();
	//	items.clear();

	for (int row = 0; row < model()->rowCount(rootIndex()); ++row) {

		QString name = model()->data( model()->index(row, 0, rootIndex()) ).toString();
		QString type = model()->data( model()->index(row, 1, rootIndex()) ).toString();

		if ( TreeItem *ti = static_cast<TreeItem *>( (model()->index(row, 0, rootIndex())).internalPointer() ) ) {

			type = ti->getType();

			QString idx =  type + "/" + name;

			qDebug() << idx;

			if ( ! items.contains(type + "/" + name) ) {
				if ( type == "eP2N" ) {
					Edge *foo = new Edge;

					foo->setSource(static_cast<Element *> (items["nFeatured/fvvkk"]));
					foo->setDest(static_cast<Element *> (items["nFeatured/fvkk2"]));

					scene->addItem(foo);
				} else {
					Element *last = new Element(type);
					items[type + "/" + name] = last;
					last->setInfo(ti->getType(), ti->getName());
					scene->addItem(last);
				}
			}
		} else { 
			qDebug("unable to cast model.internalPointer to TreeItem");
		}
	}

	//    scene->update(scene->sceneRect());
	// view->scale(1,1);
	scene->update(view->sceneRect());
}

QRect PieView::visualRect(const QModelIndex &index) const
{
qDebug() << __FUNCTION__;
	return QRect();
}

QRegion PieView::visualRegionForSelection(const QItemSelection &selection) const
{
qDebug() << __FUNCTION__;
	return QRegion();
}

int PieView::horizontalOffset() const
{
qDebug() << __FUNCTION__;
	return 0;
}

int PieView::verticalOffset() const
{
qDebug() << __FUNCTION__;
	return 0;
}
