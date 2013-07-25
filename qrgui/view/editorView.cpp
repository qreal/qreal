#include <QtWidgets>

#ifdef QT_OPENGL_LIB
#include <QtOpenGL/QGLWidget>
#endif

#include "editorView.h"

using namespace qReal;

const int boundAdjustSize = 30;

EditorView::EditorView(QWidget *parent)
	: QGraphicsView(parent), mMouseOldPosition(), mWheelPressed(false), mZoom(0)
{
	setRenderHint(QPainter::Antialiasing, true);

	mScene = new EditorViewScene(this);
	connect(mScene, SIGNAL(sceneRectChanged(QRectF)), this, SLOT(showScene()));

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
	setSceneRect(mScene->sceneRect());
	double zoomFactor = static_cast<double>(SettingsManager::value("zoomFactor").toInt()) / 10 + 1;
	scale(zoomFactor, zoomFactor);
	mZoom++;
	if (SettingsManager::value("ShowGrid").toBool()) {
		mScene->setRealIndexGrid(mScene->realIndexGrid() * zoomFactor);
	}
	checkGrid();
}

void EditorView::zoomOut()
{
	if (mWheelPressed || mZoom <= SettingsManager::value("minZoom").toInt()) {
		return;
	}
	setSceneRect(mScene->sceneRect());
	double zoomFactor = 1 / (static_cast<double>(SettingsManager::value("zoomFactor").toInt()) / 10 + 1);
	scale(zoomFactor, zoomFactor);
	mZoom--;
	if (SettingsManager::value("ShowGrid").toBool()) {
		mScene->setRealIndexGrid(mScene->realIndexGrid() * zoomFactor);
	}
	checkGrid();
}

void EditorView::checkGrid()
{
	if (SettingsManager::value("ShowGrid").toBool()) {
		if(mScene->realIndexGrid() < 2 || mScene->realIndexGrid() > 380) {
			mScene->setNeedDrawGrid(false);
		}
		else
			mScene->setNeedDrawGrid(true);
	}
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
		if (mMouseOldPosition != QPoint()) {
			QPointF offset = mapToScene(mMouseOldPosition) - mapToScene(event->pos());

			mMouseOldPosition = event->pos();

			setCenter(getCenter() + offset);
		}
	}

	QGraphicsView::mouseMoveEvent(event);
	if (event->buttons() & Qt::RightButton) {
		setDragMode(NoDrag);
	} else {
		if ((event->buttons() & Qt::LeftButton) && (event->modifiers() & Qt::ControlModifier)) {
			setDragMode(RubberBandDrag);
			mScene->itemSelectUpdate();
		} else 	if ((event->buttons() & Qt::LeftButton) && (event->modifiers() & Qt::ShiftModifier)) {
			setDragMode(ScrollHandDrag); //  (see #615)
			mScene->itemSelectUpdate();
		} else if (event->buttons() & Qt::LeftButton ) {
			EdgeElement *newEdgeEl = dynamic_cast<EdgeElement *>(itemAt(event->pos()));
			if (newEdgeEl == NULL) {
				setDragMode(RubberBandDrag);
			} else {
				if (newEdgeEl->isBreakPointPressed()) {
					newEdgeEl->breakPointUnpressed();
					setDragMode(NoDrag);
				}
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
		mMouseOldPosition = QPoint();
	}
	QGraphicsView::mouseReleaseEvent(event);
	if (mScene->getNeedDrawGrid())
		mScene->invalidate();
}

void EditorView::mousePressEvent(QMouseEvent *event)
{
	mWheelPressed  = (event->buttons() & Qt::MidButton);
	mMouseOldPosition = event->pos();
	if (!mWheelPressed) {
		QGraphicsView::mousePressEvent(event);
	}
	if ((event->buttons() & Qt::LeftButton) && (event->modifiers() & Qt::ControlModifier)) {
		setDragMode(RubberBandDrag);
		mScene->itemSelectUpdate();
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

QPointF EditorView::getCenter() const
{
	return center;
}

void EditorView::setCenter(const QPointF &centerPoint)
{
	QRectF visibleArea = mapToScene(viewport()->rect()).boundingRect();
	QRectF sceneBounds = sceneRect();

	qreal boundX = sceneBounds.x() + boundAdjustSize;
	qreal boundY = sceneBounds.y() + boundAdjustSize;
	qreal boundWidth = sceneBounds.width() - boundAdjustSize;
	qreal boundHeight = sceneBounds.height() - boundAdjustSize;

	qreal dx = (centerPoint - center).x();
	qreal dy = (centerPoint - center).y();

	QRectF bounds(boundX, boundY, boundWidth, boundHeight);

	center = centerPoint;

	if (!bounds.contains(centerPoint)) {
		if (visibleArea.contains(sceneBounds)) {
			center = sceneBounds.center();
		} else {
			if (centerPoint.x() > bounds.right()
					|| centerPoint.x() < bounds.left()) {
				moveTo(horizontal, dx);
			}
			if (centerPoint.y() > bounds.bottom()
					|| centerPoint.y() < bounds.top()) {
				moveTo(vertical, dy);
			}
		}
	}
	centerOn(center);
}

void EditorView::moveTo(EditorView::Direction direction, qreal dx)
{
	if (!dx) {
		return;
	}

	QRectF rect = sceneRect();

	if (direction == horizontal) {
		rect.moveLeft(rect.left() + dx);
		if (dx > 0) {
			rect.setLeft(rect.left() - dx);
		} else {
			rect.setRight(rect.right() - dx);
		}
	}

	if (direction == vertical) {
		rect.moveTop(rect.top() + dx);
		if (dx > 0) {
			rect.setTop(rect.top() - dx);
		} else {
			rect.setBottom(rect.bottom() - dx);
		}
	}

	setSceneRect(rect);
	emit sceneRectChanged(rect);
}

void EditorView::invalidateScene()
{
	scene()->invalidate();
}

void EditorView::ensureElementVisible(Element const * const element)
{
	if (element != NULL) {
		float const widgetWidth = size().width();
		float const widgetHeight = size().height();
		float const elementWidth = element->boundingRect().width();
		float const elementHeight = element->boundingRect().height();
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

void EditorView::showScene()
{
	if (scene() != NULL) {
		setSceneRect(sceneRect().united(mScene->sceneRect()));
	}
}
