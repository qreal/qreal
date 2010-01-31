/** @file editorviewscene.cpp
 * 	@brief Сцена для отрисовки объектов
 * */
#include "editorviewscene.h"

#include <QGraphicsTextItem>
#include <QtGui>

#include "editorviewmviface.h"
#include "editorview.h"
#include "mainwindow.h"
#include "../mainwindow/mainwindow.h"

using namespace qReal;

EditorViewScene::EditorViewScene(QObject * parent)
		:  QGraphicsScene(parent), mWindow(NULL)
{
	setItemIndexMethod(NoIndex);
	setEnabled(false);
}

void EditorViewScene::setEnabled(bool enabled)
{
	foreach (QGraphicsView *view, views())
		view->setEnabled(enabled);
}

void EditorViewScene::clearScene()
{
	foreach (QGraphicsItem *item, items())
		removeItem(item);
}

UML::Element * EditorViewScene::getElem(qReal::Id const &uuid)
{
	if (uuid == ROOT_ID)
		return NULL;

	// FIXME: SLOW!
	QList < QGraphicsItem * > list = items();
	for (QList < QGraphicsItem * >::Iterator it = list.begin(); it != list.end(); ++it) {
		if (UML::Element * elem = dynamic_cast < UML::Element * >(*it)) {
			if (elem->uuid() == uuid) {
				return elem;
			}
		}
	}

	return NULL;
}

UML::Element * EditorViewScene::getElemByModelIndex(const QModelIndex &ind)
{
	// FIXME: SLOW!
	QList < QGraphicsItem * > list = items();
	for (QList < QGraphicsItem * >::Iterator it = list.begin(); it != list.end(); ++it) {
		if (UML::Element * elem = dynamic_cast < UML::Element * >(*it)) {
			if (elem->index() == ind)
				return elem;
		}
	}
	Q_ASSERT(!"Element not found");
	return NULL;
}

void EditorViewScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
	const QMimeData *mimeData = event->mimeData();
	if (mimeData->hasFormat("application/x-real-uml-data"))
		QGraphicsScene::dragEnterEvent(event);
	else
		event->ignore();
}

void EditorViewScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
	Q_UNUSED(event);
}

void EditorViewScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
	Q_UNUSED(event);
}

void EditorViewScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
	Q_ASSERT(mWindow);  // Значение mWindow должно быть инициализировано
	// отдельно, через конструктор это делать нехорошо,
	// поскольку сцена создаётся в сгенерённом ui-шнике.

	// если нет ни одной диаграммы, то ничего не создаем.
	if (mv_iface->model()->rowCount(QModelIndex()) == 0)
		return;

	// Transform mime data to include coordinates.
	const QMimeData *mimeData = event->mimeData();
	QByteArray itemData = mimeData->data("application/x-real-uml-data");

	QDataStream in_stream(&itemData, QIODevice::ReadOnly);

	QString uuid = "";
	QString pathToItem = "";
	QString name;
	QPointF pos;

	in_stream >> uuid;
	in_stream >> pathToItem;
	in_stream >> name;
	in_stream >> pos;

	QByteArray newItemData;
	QDataStream stream(&newItemData, QIODevice::WriteOnly);

	UML::Element *newParent = NULL;

	// TODO: возможно, это можно сделать проще
	UML::Element *e = mWindow->manager()->graphicalObject(qReal::Id::loadFromString(uuid));
	//	= UML::GUIObjectFactory(type_id);

	if (dynamic_cast<UML::NodeElement*>(e))
		newParent = getElemAt(event->scenePos());

	if (e)
		delete e;

	stream << uuid;				// uuid
	stream << pathToItem;
	stream << name;

	if (!newParent)
		stream << event->scenePos();
	else
		stream << newParent->mapToItem(newParent, newParent->mapFromScene(event->scenePos()));

	QMimeData *newMimeData = new QMimeData;
	newMimeData->setData("application/x-real-uml-data", newItemData);

	QModelIndex parentIndex = newParent ? QModelIndex(newParent->index()) : mv_iface->rootIndex();

	mv_iface->model()->dropMimeData(newMimeData, event->dropAction(),
		mv_iface->model()->rowCount(parentIndex), 0, parentIndex);

	delete newMimeData;
}

void EditorViewScene::keyPressEvent(QKeyEvent *event)
{
	if (dynamic_cast<QGraphicsTextItem*>(this->focusItem())) {
		// Forward event to text editor
		QGraphicsScene::keyPressEvent(event);
	} else if (event->key() == Qt::Key_Delete) {
		// Delete selected elements from scene
		mainWindow()->deleteFromScene();
	} else
		QGraphicsScene::keyPressEvent(event);
}

void EditorViewScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	// Let scene update selection and perform other operations
	QGraphicsScene::mousePressEvent(event);

	if (event->button() != Qt::RightButton)
		return;

	UML::Element *e = getElemAt(event->scenePos());
	if (!e)
		return;
	if (!e->isSelected()) {
		clearSelection();
		e->setSelected(true);
	}

	// Menu belongs to scene handler because it can delete elements.
	// We cannot not allow elements to commit suicide.

	QMenu menu;
	menu.addAction(mWindow->ui.actionDeleteFromDiagram);
	QList<UML::ContextMenuAction*> elementActions = e->contextMenuActions();

	if (!elementActions.isEmpty())
		menu.addSeparator();

	foreach (UML::ContextMenuAction* action, elementActions) {
		action->setEventPos(e->mapFromScene(event->scenePos()));
		menu.addAction(action);
	}

	// FIXME: add check for diagram
	//	if (selectedItems().count() == 1)
	//		menu.addAction(window->ui.actionJumpToAvatar);

	menu.exec(QCursor::pos());
}

void EditorViewScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		// Double click on a title activates it
		if (UML::ElementTitle *title = dynamic_cast<UML::ElementTitle*>(itemAt(event->scenePos()))) {
			if (!title->hasFocus()) {  // Do not activate already activated item
				event->accept();
				title->startTextInteraction();
				return;
			}
		}
		else if (UML::NodeElement *element = dynamic_cast<UML::NodeElement*>(itemAt(event->scenePos()))) {
			event->accept();
			mainWindow()->activateSubdiagram(element->index());
			// Now scene is changed from outside. Being a mere mortal I do not
			// know whether it is good or not, but what is the destiny of
			// workflow after this return?
			return;
		}
	}

	QGraphicsScene::mouseDoubleClickEvent(event);
}

UML::Element* EditorViewScene::getElemAt(QPointF const &position)
{
	foreach (QGraphicsItem *item, items(position)) {
		UML::Element *e = dynamic_cast<UML::Element *>(item);
		if (e)
			return e;
	}
	return NULL;
}

QPersistentModelIndex EditorViewScene::rootItem()
{
	return mv_iface->rootIndex();
}

void EditorViewScene::setMainWindow(qReal::MainWindow *mainWindow)
{
	mWindow = mainWindow;
}

qReal::MainWindow *EditorViewScene::mainWindow() const
{
	return mWindow;
}
