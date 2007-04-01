#include <QtGui>
#include <QtOpenGL/QGLWidget>

#include "editorview.h"

EditorView::EditorView(QWidget *parent)
    : QGraphicsView(parent)
{
    setScene(new EditorViewScene(this));
    mvciface = new EditorViewMViface(this,dynamic_cast<EditorViewScene *>(scene()));

//    setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    setRenderHints(QPainter::Antialiasing);
    
    QGraphicsView *minimap = new QGraphicsView(scene());
    minimap->setWindowTitle("Minimap");
    minimap->resize(200,200);
    minimap->scale(0.2,0.2);
    minimap->show();
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

