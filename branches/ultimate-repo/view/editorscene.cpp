/** @file editorviewscene.cpp
 * 	@brief Сцена для отрисовки объектов
 * */
#include "editorscene.h"
#include "editorview.h"
#include "uml_nodeelement.h"
#include "uml_guiobjectfactory.h"
#include "mainwindow.h"

#include <QGraphicsTextItem>
#include <QtGui>
#include "../common/classes.h"

extern MainWindow *window;

EditorScene::EditorScene(QObject *parent) : QGraphicsScene(parent)
{
	setItemIndexMethod(NoIndex);
}

#if 0
void EditorScene::dragEnterEvent ( QGraphicsSceneDragDropEvent * event )
{
	Q_UNUSED(event);
	//	event->setAccepted();
}

void EditorScene::dragMoveEvent ( QGraphicsSceneDragDropEvent * event )
{
	Q_UNUSED(event);
}

void EditorScene::dragLeaveEvent ( QGraphicsSceneDragDropEvent * event )
{
	Q_UNUSED(event);
}
#endif

void EditorScene::dropEvent ( QGraphicsSceneDragDropEvent * event )
{
	// Transform mime data to include coordinates.
	const QMimeData *mimeData = event->mimeData();
	QByteArray itemData = mimeData->data("application/x-real-uml-data");

	QDataStream in_stream(&itemData, QIODevice::ReadOnly);

	IdType uuid = "", oldparent = "";
	TypeIdType type_id = "";
	QString name;
	QPointF pos;

	in_stream >> uuid;
	in_stream >> type_id;
	in_stream >> oldparent;
	in_stream >> name;
	in_stream >> pos;

	QByteArray newItemData;
	QDataStream stream(&newItemData, QIODevice::WriteOnly);

	UML::Element *newParent = 0;

	UML::Element *e = UML::GUIObjectFactory(type_id);

	if (dynamic_cast<UML::NodeElement *>(e)) {
		newParent= getElemAt(event->scenePos());
	}

	if (e) {
		delete e;
	}

	stream << uuid;				// uuid
	stream << type_id;			// type
	stream << oldparent;
	stream << name;

	if (!newParent) {
		stream << event->scenePos();
	} else {
		stream << newParent->mapToItem(newParent, newParent->mapFromScene(event->scenePos()));
	}

	QMimeData *newMimeData = new QMimeData;
	newMimeData->setData("application/x-real-uml-data", newItemData);

	if (newParent) {
		mv_iface->model()->dropMimeData( newMimeData, event->dropAction(),
						 mv_iface->model()->rowCount(newParent->index()), 0, newParent->index() );
	} else {
		mv_iface->model()->dropMimeData( newMimeData, event->dropAction(),
						 mv_iface->model()->rowCount(mv_iface->rootIndex()), 0, mv_iface->rootIndex() );
	}
	delete newMimeData;
	updateLinks();
}

void EditorScene::keyPressEvent( QKeyEvent * event )
{
	if ((event->key() == Qt::Key_Return) && (this->focusItem()!= NULL)){
		this->focusItem()->clearFocus();
	} else if (event->key() == Qt::Key_Delete) {
		QGraphicsTextItem *ti = NULL;
		if (this->focusItem()!= NULL)
			ti = dynamic_cast<QGraphicsTextItem *>(this->focusItem());
		if (ti)
		{
			// text item has focus. Just pass key to it
			QGraphicsScene::keyPressEvent(event);
		}
		else // Add more cases if necessary
		{
			// then uml element has focus, we can safely delete it.
			window->deleteFromDiagram();
		}
	} else
		QGraphicsScene::keyPressEvent(event);
}

void EditorScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	// Let scene update selection and perform other operations
	QGraphicsScene::mousePressEvent(event);

	if (event->button() != Qt::RightButton)
		return;

	// Did we click on element?
	UML::Element *e = dynamic_cast<UML::Element*>(itemAt(event->scenePos()));
	if (!e) return;

	// Skip edges
	if (dynamic_cast<UML::EdgeElement*>(e))
		return;

	// Menu belongs to scene handler because it can delete elements.
	// We cannot not allow elements to commit suicide.
	QMenu menu;
	menu.addAction(window->ui.actionDeleteFromDiagram);
	// FIXME: add check for diagram
	if (selectedItems().count() == 1)
		menu.addAction(window->ui.actionJumpToAvatar);

	menu.exec(QCursor::pos());
}

void EditorScene::updateLinks()
{
	foreach (QGraphicsItem *item, items())
	{
		UML::EdgeElement *e = dynamic_cast<UML::EdgeElement *>(item);
		if (e)
			e->makeRightAngle();
	}
}

void EditorScene::setModel(QAbstractItemModel *model)
{
	mv_iface->setModel(model);
}

/* {{{ EditorViewMViface*/
EditorScene::EditorViewMViface::EditorViewMViface(EditorView *view, EditorViewScene *scene)
	: QAbstractItemView(0)
{
	this->view = view;
	this->scene = scene;

	//    view->mv_iface = this;
	scene->mv_iface = this;
	scene->view = view;
}

QRect EditorScene::EditorViewMViface::visualRect(const QModelIndex &) const
{
	return QRect();
}

void EditorScene::EditorViewMViface::scrollTo(const QModelIndex &, ScrollHint)
{
}

QModelIndex EditorScene::EditorViewMViface::indexAt(const QPoint &) const
{
	return QModelIndex();
}

QModelIndex EditorScene::EditorViewMViface::moveCursor(QAbstractItemView::CursorAction,
		Qt::KeyboardModifiers)
{
	return QModelIndex();
}

int EditorScene::EditorViewMViface::horizontalOffset() const
{
	return 0;
}

int EditorScene::EditorViewMViface::verticalOffset() const
{
	return 0;
}

bool EditorScene::EditorViewMViface::isIndexHidden(const QModelIndex &) const
{
	return false;
}

void EditorScene::EditorViewMViface::setSelection(const QRect&, QItemSelectionModel::SelectionFlags )
{
}

QRegion EditorScene::EditorViewMViface::visualRegionForSelection(const QItemSelection &) const
{
	return QRegion();
}

void EditorScene::EditorViewMViface::reset()
{
	items.clear();
	scene->clearScene();

	// so that our diagram be nicer
	QGraphicsRectItem *rect = scene->addRect(QRect(-1000,-1000,2000,2000));
	scene->removeItem(rect);
	delete rect;

	if ( model() )
		rowsInserted(rootIndex(), 0, model()->rowCount(rootIndex()) - 1 );
}

void EditorScene::EditorViewMViface::setRootIndex(const QModelIndex &index)
{
	QAbstractItemView::setRootIndex(index);
	reset();
}

void EditorScene::EditorViewMViface::rowsInserted(const QModelIndex &parent, int start, int end)
{
	qDebug() << "========== rowsInserted" << parent << start << end;

	if (parent == QModelIndex() || parent.parent() == QModelIndex())
		return;

	qDebug() << "rowsInserted: adding items" << parent;
	for (int row = start; row <= end; ++row) {
		QPersistentModelIndex current = model()->index(row, 0, parent);
		IdType uuid = current.data(Unreal::IdRole).toString();
		TypeIdType type = current.data(Unreal::TypeRole).toString();

		IdType parent_uuid = "";
		if (parent != rootIndex())
			parent_uuid = parent.data(Unreal::IdRole).toString();

		qDebug() << uuid << type;

		if (uuid == "")
			continue;

		if (UML::Element *e = UML::GUIObjectFactory(type)) {
			scene->addItem(e);
			e->setIndex(current);
			e->setPos(current.data(Unreal::PositionRole).toPointF());

			if (parent_uuid != "")
				e->setParentItem(items[parent]);

			items[current] = e;

			e->updateData();
			e->connectToPort();
		}

		if (model()->hasChildren(current)) {
			rowsInserted(current, 0, model()->rowCount(current) - 1);
		}
	}

	QAbstractItemView::rowsInserted(parent, start, end);
}

void EditorScene::EditorViewMViface::rowsAboutToBeRemoved ( const QModelIndex & parent, int start, int end )
{
	for (int row = start; row <= end; ++row) {
		QModelIndex curr = model()->index(row, 0, parent);
		scene->removeItem(items[curr]);
		delete items[curr];
		items.remove(curr);
	}

	QAbstractItemView::rowsAboutToBeRemoved(parent, start, end);
}

void EditorScene::EditorViewMViface::dataChanged(const QModelIndex &topLeft,
	const QModelIndex &bottomRight)
{
	for (int row = topLeft.row(); row <= bottomRight.row(); ++row) {
		QModelIndex curr = topLeft.sibling(row, 0);
		if (items.contains(curr)) {
			Q_ASSERT(items[curr] != NULL);
			items[curr]->updateData();
		}
		else
			rowsInserted(topLeft.parent(),row,row);
	}
}
/* }}} */
