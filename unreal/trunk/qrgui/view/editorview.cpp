#include <QtGui>

#ifdef QT_OPENGL_LIB
#include <QtOpenGL/QGLWidget>
#endif

#include "editorview.h"

using namespace qReal;

EditorView::EditorView(QWidget *parent)
	: QGraphicsView(parent)
{
	setRenderHint(QPainter::Antialiasing, true);

	mScene = new EditorViewScene(this);
	mMVIface = new EditorViewMViface(this, mScene);
	setScene(mScene);

	setAcceptDrops(true);
	setDragMode(RubberBandDrag);
	setEnabled(false);

	setMouseTracking(true);
}

EditorView::~EditorView()
{
	delete mMVIface;
	delete mScene;
}

void EditorView::toggleAntialiasing(bool checked)
{
	setRenderHint(QPainter::Antialiasing, checked);
	setRenderHint(QPainter::SmoothPixmapTransform, checked);
}

void EditorView::toggleOpenGL(bool checked)
{
	Q_UNUSED(checked)
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

void EditorView::setMainWindow(qReal::MainWindow *mainWindow) {
	mMVIface->scene()->setMainWindow(mainWindow);
}

void EditorView::changeSceneGrid()
{
	mScene->changeNeedDrawGrid();
	mScene->invalidate();
}

void EditorView::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::RightButton)
		setDragMode(NoDrag);
	else
		setDragMode(RubberBandDrag);
	QGraphicsView::mouseMoveEvent(event);
}

