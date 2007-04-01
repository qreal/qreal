#include <QtGui>
#include <QtOpenGL/QGLWidget>

#include "editorview.h"

EditorView::EditorView(QWidget *parent)
    : QGraphicsView(parent)
{
    EditorViewScene *myScene = new EditorViewScene(this);
    mv_iface = new EditorViewMViface(this,myScene);
    setScene(myScene);

//    setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    setRenderHints(QPainter::Antialiasing);
    
    setAcceptDrops(true);
}

EditorView::~EditorView()
{
}

void EditorView::mousePressEvent(QMouseEvent *event)
{
    if (QGraphicsItem *item = itemAt(event->pos())) {
	mv_iface->raiseClick(item);
    }

    QGraphicsView::mousePressEvent(event);
}
