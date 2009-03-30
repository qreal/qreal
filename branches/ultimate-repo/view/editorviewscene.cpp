/** @file editorviewscene.cpp
 * 	@brief Сцена для отрисовки объектов
 * */
#include "editorviewscene.h"
#include "editorviewmviface.h"
#include "editorview.h"
#include "uml_nodeelement.h"
#include "uml_guiobjectfactory.h"
#include "mainwindow.h"


#include <QGraphicsTextItem>
#include <QtGui>
#include "../common/classes.h"

extern MainWindow *window;

EditorViewScene::EditorViewScene(QObject * parent)
	:  QGraphicsScene(parent)
{
//	setSceneRect(-400, -300, 800, 600);
	setItemIndexMethod(NoIndex);
//	setBackgroundBrush(gradient);
}

void EditorViewScene::clearScene()
{
	QList < QGraphicsItem * >list = items();
	QList < QGraphicsItem * >::Iterator it = list.begin();
	for (; it != list.end(); ++it) {
		removeItem(*it);
	}
}

UML::Element * EditorViewScene::getElem(IdType const &uuid)
{
	// FIXME: SLOW!
	QList < QGraphicsItem * > list = items();
	for (QList < QGraphicsItem * >::Iterator it = list.begin(); it != list.end(); ++it) {
		if (UML::Element * elem = dynamic_cast < UML::Element * >(*it)) {
			if (elem->uuid() == uuid) {
				return elem;
			}
		}
	}
	qDebug() << uuid;
	Q_ASSERT(uuid == INVALID_ID);
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

void EditorViewScene::dragEnterEvent ( QGraphicsSceneDragDropEvent * event )
{
	Q_UNUSED(event);
//	event->setAccepted();
}

void EditorViewScene::dragMoveEvent ( QGraphicsSceneDragDropEvent * event )
{
	Q_UNUSED(event);
}

void EditorViewScene::dragLeaveEvent ( QGraphicsSceneDragDropEvent * event )
{
	Q_UNUSED(event);
}

void EditorViewScene::dropEvent ( QGraphicsSceneDragDropEvent * event )
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
}

void EditorViewScene::keyPressEvent( QKeyEvent * event )
{      
    if ((event->key() == Qt::Key_Return) && (this->focusItem()!= NULL)){
        this->focusItem()->clearFocus();
    } else

    if (event->key() == Qt::Key_Delete)
    {
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
            // then uml element has focus, then we can safely delete it.
            window->deleteFromDiagram();
        }
    } else 
         QGraphicsScene::keyPressEvent(event);
}

void EditorViewScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

	do
	{
            if (event->button() == Qt::LeftButton)
            {
                UML::Element *e = getElemAt(event->scenePos());
                if (!e)
                    break;

                if (!e->isSelected())
                {
                        clearSelection();
                        e->setSelected(true);
                }
            }

            if (event->button() != Qt::RightButton)
                break;

                /*UML::Element *e = getElemAt(event->scenePos());
                if (!e)
                    break;


                if (!e->isSelected())
		{
			clearSelection();
			e->setSelected(true);
                }*/
            QMenu menu;
            menu.addAction(window->ui.actionDeleteFromDiagram);
		// FIXME: add check for diagram
            if (selectedItems().count() == 1)
                menu.addAction(window->ui.actionJumpToAvatar);

            menu.exec(QCursor::pos());
            return;
            } while (0);
            QGraphicsScene::mousePressEvent(event);
}

UML::Element * EditorViewScene::getElemAt( const QPointF &position )
{
	foreach( QGraphicsItem *item, items(position) ) {
		UML::Element *e = dynamic_cast<UML::Element *>(item);
		if ( e )
			return e;
	}
	return 0;
}

QPersistentModelIndex EditorViewScene::rootItem()
{
	return mv_iface->rootIndex();
}
