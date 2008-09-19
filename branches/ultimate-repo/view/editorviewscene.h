#ifndef EDITORVIEWSCENE_H
#define EDITORVIEWSCENE_H

#include <QtGui/QGraphicsScene>

#include "uml_element.h"

class EditorViewMViface;
class EditorView;

class EditorViewScene : public QGraphicsScene
{
    Q_OBJECT

public:
    EditorViewScene(QObject *parent = 0);

    void clearScene();
    
    UML::Element *getElem(int uuid);
    UML::Element *getElemByModelIndex(const QModelIndex&);
    
protected:
    void dragEnterEvent( QGraphicsSceneDragDropEvent *event );
    void dragMoveEvent( QGraphicsSceneDragDropEvent *event );
    void dragLeaveEvent( QGraphicsSceneDragDropEvent *event );
    void dropEvent ( QGraphicsSceneDragDropEvent *event );

    void keyPressEvent( QKeyEvent *event );
private:
	QPointF newElementsPosition;

    EditorViewMViface *mv_iface;
    EditorView *view;
    
    friend class EditorViewMViface;
};

#endif
