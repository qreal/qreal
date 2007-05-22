#include <QtGui>

#ifdef QT_OPENGL_LIB
#include <QtOpenGL/QGLWidget>
#endif

#include "editorview.h"

EditorView::EditorView(QWidget *parent)
    : QGraphicsView(parent)
{
	setRenderHint(QPainter::Antialiasing, true);

    EditorViewScene *myScene = new EditorViewScene(this);
    mv_iface = new EditorViewMViface(this,myScene);
    setScene(myScene);

    setAcceptDrops(true);
}

EditorView::~EditorView()
{
}
/*
void EditorView::mousePressEvent(QMouseEvent *event)
{
//    if (QGraphicsItem *item = itemAt(event->pos())) {
//	mv_iface->raiseClick(item);
//    }

    QGraphicsView::mousePressEvent(event);
}
*/
void EditorView::toggleAntialiasing(bool checked)
{
    setRenderHint(QPainter::Antialiasing, checked);
    setRenderHint(QPainter::SmoothPixmapTransform, checked);
}

void EditorView::toggleOpenGL(bool checked)
{
#ifdef QT_OPENGL_LIB
    setViewport(checked ? new QGLWidget(QGLFormat(QGL::SampleBuffers)) : new QWidget);
#endif
}

void EditorView::zoomIn()
{
    scale(1.5,1.5);
}

void EditorView::zoomOut()
{
    scale(0.666,0.666);
}

