#include <QtWidgets>

#ifdef QT_OPENGL_LIB
#include <QtOpenGL/QGLWidget>
#endif

#include "editorView.h"

using namespace qReal;

EditorView::EditorView(QWidget *parent)
	: QGraphicsView(parent)
	, mMouseOldPosition()
	, mWheelPressed(false)
	, mZoom(0)
	, mTouchManager(this)
{
	setRenderHint(QPainter::Antialiasing, true);

	mScene = new EditorViewScene(this);
	connect(mScene, SIGNAL(zoomIn()), this, SLOT(zoomIn()));
	connect(mScene, SIGNAL(zoomOut()), this, SLOT(zoomOut()));

	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	setResizeAnchor(QGraphicsView::AnchorUnderMouse);

	mMVIface = new EditorViewMViface(this, mScene);
	setScene(mScene);

	setAcceptDrops(true);
	setDragMode(RubberBandDrag);
	setEnabled(false);

	setMouseTracking(true);

	setAlignment(Qt::AlignCenter);

	connect(&mTouchManager, SIGNAL(gestureStarted()), mScene, SLOT(deleteGesture()));
}

EditorView::~EditorView()
{
	delete mMVIface;
	delete mScene;
}

EditorViewMViface *EditorView::mvIface() const
{
	return mMVIface;
}

EditorViewScene *EditorView::editorViewScene() const
{
	return mScene;
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
	if (mWheelPressed || mZoom >= SettingsManager::value("maxZoom").toInt()) {
		return;
	}

	startAnimation(SLOT(zoomInTime()));
	++mZoom;
}

void EditorView::zoomOut()
{
	if (mWheelPressed || mZoom <= SettingsManager::value("minZoom").toInt()) {
		return;
	}

	startAnimaition(SLOT(zoomOutTime()));
	--mZoom;
}

void EditorView::checkGrid()
{
	if (SettingsManager::value("ShowGrid").toBool()) {
		mScene->setNeedDrawGrid(mScene->realIndexGrid() >= 2 && mScene->realIndexGrid() <= 380);
	}
}

void EditorView::startAnimation(char const *slot)
{
	QTimeLine *anim = new QTimeLine(300, this);
	anim->setUpdateInterval(20);

	connect(anim, SIGNAL(valueChanged(qreal)), this, slot);
	connect(anim, SIGNAL(finished()), this, SLOT(animFinished()));
	anim->start();
}

void EditorView::setMainWindow(qReal::MainWindow *mainWindow)
{
	mMVIface->scene()->setMainWindow(mainWindow);
}

void EditorView::setDrawSceneGrid(bool show)
{
	mScene->setNeedDrawGrid(show);
	mScene->invalidate();
}

void EditorView::mouseMoveEvent(QMouseEvent *event)
{
	if (mWheelPressed) {
		if (mMouseOldPosition != QPointF()) {
			QRectF rect = sceneRect();
			qreal dx = (event->localPos().x() - mMouseOldPosition.x());
			qreal dy = (event->localPos().y() - mMouseOldPosition.y());
			rect.moveLeft(rect.left() - dx);
			rect.moveTop(rect.top() - dy);
			setSceneRect(rect);
			translate(dx, dy);
		}
		mMouseOldPosition = event->localPos();
	}
	QGraphicsView::mouseMoveEvent(event);
	if (event->buttons() & Qt::RightButton) {
		setDragMode(NoDrag);
	} else {
		if ((event->buttons() & Qt::LeftButton) && (event->modifiers() & Qt::ControlModifier)) {
			setDragMode(RubberBandDrag);
			mScene->itemSelectUpdate();
		/*} else 	if ((event->buttons() & Qt::LeftButton) && (event->modifiers() & Qt::ShiftModifier)) {
			setDragMode(ScrollHandDrag); //  (see #615)
			mScene->itemSelectUpdate();*/
		} else if (event->buttons() & Qt::LeftButton ) {
			EdgeElement *newEdgeEl = dynamic_cast<EdgeElement *>(itemAt(event->pos()));
			if (newEdgeEl && newEdgeEl->isBreakPointPressed()) {
				newEdgeEl->breakPointUnpressed();
				setDragMode(NoDrag);
			}
		}
	}
	if (mScene->getNeedDrawGrid())
		mScene->invalidate();
}

void EditorView::mouseReleaseEvent(QMouseEvent *event)
{
	if (!(event->buttons() & Qt::MidButton)) {
		mWheelPressed = false;
		mMouseOldPosition = QPointF();
	}
	QGraphicsView::mouseReleaseEvent(event);
	if (mScene->getNeedDrawGrid()) {
		mScene->invalidate();
	}
}

void EditorView::mousePressEvent(QMouseEvent *event)
{
	mWheelPressed  = (event->buttons() & Qt::MidButton);
	mMouseOldPosition = QPointF();
	if (!mWheelPressed) {
		QGraphicsView::mousePressEvent(event);
	}
	if (event->buttons() & Qt::LeftButton) {
		if (!(event->buttons() & Qt::RightButton) && !mTouchManager.isGestureRunning()) {
			setDragMode(RubberBandDrag);
		}
		if (event->modifiers() & Qt::ControlModifier) {
			mScene->itemSelectUpdate();
		}
	}
}

void EditorView::scrollContentsBy(int dx, int dy)
{
	QGraphicsView::scrollContentsBy(dx, dy);
	if (mScene->getNeedDrawGrid()) {
		mScene->invalidate();
	}
}

void EditorView::keyPressEvent(QKeyEvent *event)
{
	QGraphicsView::keyPressEvent(event);
	if (event->key() == Qt::Key_Space) {
		setDragMode(QGraphicsView::ScrollHandDrag);
		event->accept();
	}
}

void EditorView::keyReleaseEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Space) {
		setDragMode(QGraphicsView::RubberBandDrag);
	} else {
		QGraphicsView::keyPressEvent(event);
	}
}

bool EditorView::viewportEvent(QEvent *event)
{
	switch (event->type()) {
	case QEvent::TouchBegin:
	case QEvent::TouchUpdate:
	case QEvent::TouchEnd:
		// For some reason touch viewport events can`t be processed in manual event
		// filters, so catching them here
		return mTouchManager.processTouchEvent(static_cast<QTouchEvent *>(event));
	default:
		break;
	}
	return QGraphicsView::viewportEvent(event);
}

void EditorView::invalidateScene()
{
	scene()->invalidate();
}

void EditorView::ensureElementVisible(Element const * const element)
{
	if (element) {
		qreal const widgetWidth = size().width();
		qreal const widgetHeight = size().height();
		qreal const elementWidth = element->boundingRect().width();
		qreal const elementHeight = element->boundingRect().height();
		ensureVisible(element, (widgetWidth - elementWidth) / 2, (widgetHeight - elementHeight) / 2);
	}
}

void EditorView::ensureElementVisible(Element const * const element
		, int xMargin, int yMargin)
{
	if (element) {
		ensureVisible(element, xMargin, yMargin);
	}
}

void EditorView::setTitlesVisible(bool visible)
{
	mScene->setTitlesVisible(visible);
}

void EditorView::zoomInTime()
{
	qreal const zoomFactor = static_cast<qreal>(SettingsManager::value("zoomFactor").toInt()) / (10 + 15) + 1;
	zoom(zoomFactor);
}

void EditorView::zoomOutTime()
{
	qreal const zoomFactor = 1 / (static_cast<qreal>(SettingsManager::value("zoomFactor").toInt()) / (10 + 15) + 1);
	zoom(zoomFactor);
}

void EditorView::animFinished()
{
	delete sender();
}

void EditorView::zoom(qreal const zoomFactor)
{
	setSceneRect(mScene->sceneRect());
	scale(zoomFactor, zoomFactor);

	if (SettingsManager::value("ShowGrid").toBool()) {
		mScene->setRealIndexGrid(mScene->realIndexGrid() * zoomFactor);
	}
	checkGrid();
}
