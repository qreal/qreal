#include "editorviewscene.h"

#include  "element.h"

EditorViewScene::EditorViewScene(QObject *parent)
    : QGraphicsScene(parent)
{
    Element *e = new Element(0);
    addItem(e);
}

void EditorViewScene::clearScene ()
{
    QList<QGraphicsItem *> list = items();
    QList<QGraphicsItem *>::Iterator it = list.begin();
    for (; it != list.end(); ++it) {
            if ( *it )
                    delete *it;
    }
}
