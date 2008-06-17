#include "editorviewscene.h"
#include "editorviewmviface.h"
#include "editorview.h"

#include <QGraphicsTextItem>
#include <QtGui>


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

UML::Element * EditorViewScene::getElem(int uuid)
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
	return 0;
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

	int uuid, type_id;
	QString name;
	QPointF pos;

	in_stream >> uuid;
	in_stream >> type_id;
	in_stream >> name;
	in_stream >> pos;

	QByteArray newItemData;
	QDataStream stream(&newItemData, QIODevice::WriteOnly);

	stream << uuid;				// uuid
	stream << type_id;			// type
	stream << name;
	stream << event->scenePos();

	QMimeData *newMimeData = new QMimeData;
	newMimeData->setData("application/x-real-uml-data", newItemData);

	mv_iface->model()->dropMimeData( newMimeData, event->dropAction(),
			mv_iface->model()->rowCount(mv_iface->rootIndex()), 0, mv_iface->rootIndex() );
	delete newMimeData;
}

void EditorViewScene::keyPressEvent( QKeyEvent * event )
{
	if ( event->key() == Qt::Key_Delete ) {
		QList<QGraphicsItem *> list = selectedItems();
		foreach ( QGraphicsItem *item, list ) {
			if ( UML::Element *e = dynamic_cast<UML::Element *>(item) ) {
				
			}
		}
	}

	QGraphicsScene::keyPressEvent(event);
}
