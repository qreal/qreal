#ifndef EDITORVIEWSCENE_H
#define EDITORVIEWSCENE_H

#include <QtGui/QGraphicsScene>

#include "uml_element.h"

class EditorViewScene : public QGraphicsScene
{
    Q_OBJECT

public:
    EditorViewScene(QObject *parent = 0);

    void clearScene();
    
    UML::Element *getElem(int uuid);
    
protected:
    void dragEnterEvent( QGraphicsSceneDragDropEvent * event );
    void dropEvent ( QGraphicsSceneDragDropEvent * event );
    
};

#endif
