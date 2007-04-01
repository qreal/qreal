#include <QtGui>
#include <QtOpenGL/QGLWidget>

#include "editorview.h"

EditorView::EditorView(QWidget *parent)
    : QGraphicsView(parent)
{
    EditorViewScene *myScene = new EditorViewScene(this);
    mvciface = new EditorViewMViface(this,myScene);
    setScene(myScene);

//    setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    setRenderHints(QPainter::Antialiasing);
}

EditorView::~EditorView()
{
}

void EditorView::mousePressEvent(QMouseEvent *event)
{
    if (QGraphicsItem *item = itemAt(event->pos())) {
	mvciface->raiseClick(item);
    }

    QGraphicsView::mousePressEvent(event);
}

