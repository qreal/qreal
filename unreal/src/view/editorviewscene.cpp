#include "editorviewscene.h"

#include  "element.h"

#include <QGraphicsTextItem>
#include <QtGui>


EditorViewScene::EditorViewScene(QObject * parent)
	:  QGraphicsScene(parent)
{
	setSceneRect(-400, -300, 800, 600);
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
	qDebug() << "FIXME: SLOW!";
	QList < QGraphicsItem * > list = items();
	for (QList < QGraphicsItem * >::Iterator it = list.begin(); it != list.end(); ++it) {
		if (UML::Element * elem = dynamic_cast < UML::Element * >(*it)) {
			qDebug() << "uuid" << elem->uuid() << "wanted" <<uuid;
			if (elem->uuid() == uuid) {
				return elem;
			}
		}
	}
	qDebug() << "uuid" << uuid << "not found";
	return 0;
}
