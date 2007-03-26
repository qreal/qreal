#ifndef EDITORVIEWSCENE_H
#define EDITORVIEWSCENE_H

#include <QtGui/QGraphicsScene>

class EditorViewScene : public QGraphicsScene
{
    Q_OBJECT

public:
    EditorViewScene(QObject *parent = 0);

    void clearScene();
};

#endif
