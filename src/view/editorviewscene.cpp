#include "editorviewscene.h"
#include "editorviewmviface.h"
#include "editorview.h"

#include "diagramexplorermodel.h"

#include <QGraphicsTextItem>
#include <QtGui>


EditorViewScene::EditorViewScene(QObject * parent)
	:  QGraphicsScene(parent)
{
//	setSceneRect(-400, -300, 800, 600);
}

void EditorViewScene::clearScene()
{
	QList < QGraphicsItem * >list = items();
	QList < QGraphicsItem * >::Iterator it = list.begin();
	for (; it != list.end(); ++it) {
		if (*it)
			delete *it;
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
	event->setAccepted(event->mimeData()->hasText());
}

void EditorViewScene::dragMoveEvent ( QGraphicsSceneDragDropEvent * event )
{
}

void EditorViewScene::dragLeaveEvent ( QGraphicsSceneDragDropEvent * event )
{
}

void EditorViewScene::dropEvent ( QGraphicsSceneDragDropEvent * event )
{
    static int count;

    if ( event->mimeData()->hasText() ) {
	if ( event->mimeData()->text() == "nFeatured" ) {
		QList<QString> list;

	        if (view->getDEM()->elementExists(QString("class %1").arg(++count), "nFeatured", "req_diagram_1", true) <= 0)
	                return;

		list << QString("class %1").arg(count) << "nFeatured" << "req_diagram_1" 
			<< QString::number(event->scenePos().x())
			<< QString::number(event->scenePos().y()) << "" << "" << "0" << "";
qDebug() << list;
		QString fields = "uuid, name, description, priority, source, status, diagram";
	
	        view->getDEM()->insert(true, fields, list);
	} else if ( event->mimeData()->text() == "eP2N" ) {
		QList<QString> list;
		
	        list << QString("link %1").arg(count++) << "eP2N" << "req_diagram_1"
			<< QString::number(event->scenePos().x())
                        << QString::number(event->scenePos().y()) << "" << "0" << "0";
	
		QString fields = "uuid, name, beginsWith, endsWith, status, diagram";

	        view->getDEM()->insert(true, fields, list);

	}
    }
}

void EditorViewScene::keyPressEvent( QKeyEvent * event )
{
    if ( event->key() == Qt::Key_Delete ) {
        QList<QGraphicsItem *> list = selectedItems();
        foreach ( QGraphicsItem *item, list ) {
            if ( UML::Element *e = dynamic_cast<UML::Element *>(item) ) {
		QStringList list;
		list << e->index().sibling(e->index().row(),1).data().toString()
			<< e->index().sibling(e->index().row(),2).data().toString()
			<< e->index().sibling(e->index().row(),3).data().toString();

		view->getDEM()->remove(true, list);
	    }
        }
    }
    
    QGraphicsScene::keyPressEvent(event);
}
