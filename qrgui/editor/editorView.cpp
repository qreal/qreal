#include "editorView.h"

#include <QtCore/QTimeLine>
#include <QtGui/QFontDatabase>

#include <qrkernel/settingsListener.h>
#include <qrutils/mathUtils/math.h>
#include <qrgui/models/models.h>

using namespace qReal;

const int zoomAnimationInterval = 20;
const int zoomAnimationTimes = 4;

EditorView::EditorView(models::Models const &models
		, Controller &controller
		, SceneCustomizer const &customizer
		, const Id &rootId
		, QWidget *parent)
	: QGraphicsView(parent)
	, mScene(models, controller, customizer, rootId, this)
	, mMVIface(this, &mScene)
	, mMouseOldPosition()
	, mWheelPressed(false)
	, mTouchManager(this)
{
	setRenderHint(QPainter::Antialiasing, true);

	connect(&mScene, SIGNAL(zoomIn()), this, SLOT(zoomIn()));
	connect(&mScene, SIGNAL(zoomOut()), this, SLOT(zoomOut()));
	connect(&mScene, &EditorViewScene::sceneRectChanged, this
			, static_cast<void (EditorView::*)(const QRectF &)>(&EditorView::setSceneRect));

	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	setResizeAnchor(QGraphicsView::AnchorUnderMouse);

	setScene(&mScene);

	setAcceptDrops(true);
	setDragMode(RubberBandDrag);
	setEnabled(false);

	setMouseTracking(true);

	setAlignment(Qt::AlignCenter);

	connect(&mTouchManager, SIGNAL(gestureStarted()), &mScene, SLOT(deleteGesture()));
	SettingsListener::listen("IndexGrid", &mScene, &EditorViewScene::redraw);
	SettingsListener::listen("GridWidth", &mScene, &EditorViewScene::redraw);
	SettingsListener::listen("CurrentFont", this, &EditorView::setSceneFont);

	addAction(&mScene.deleteAction());
	addActions(mScene.editorActions());
}

EditorViewMViface const &EditorView::mvIface() const
{
	return mMVIface;
}

EditorViewMViface &EditorView::mutableMvIface()
{
	return mMVIface;
}

void EditorView::focusOutEvent(QFocusEvent *event)
{
	if (event->reason() != Qt::PopupFocusReason) {
		mScene.setActionsEnabled(false);
	}
}

void EditorView::focusInEvent(QFocusEvent *event)
{
	Q_UNUSED(event)
	mScene.setActionsEnabled(true);
}

EditorViewScene const &EditorView::editorViewScene() const
{
	return mScene;
}

EditorViewScene &EditorView::mutableScene()
{
	return mScene;
}

void EditorView::toggleAntialiasing(bool checked)
{
	setRenderHint(QPainter::Antialiasing, checked);
	setRenderHint(QPainter::SmoothPixmapTransform, checked);
}

void EditorView::zoomIn()
{
	if (!mWheelPressed) {
		startAnimation(SLOT(zoomInTime()));
	}
}

void EditorView::zoomOut()
{
	if (!mWheelPressed) {
		startAnimation(SLOT(zoomOutTime()));
	}
}

void EditorView::checkGrid()
{
	if (SettingsManager::value("ShowGrid").toBool()) {
		mScene.setNeedDrawGrid(mScene.realIndexGrid() >= 2 && mScene.realIndexGrid() <= 380);
	}
}

void EditorView::startAnimation(char const *slot)
{
	QTimeLine *anim = new QTimeLine(zoomAnimationTimes * zoomAnimationInterval, this);
	anim->setUpdateInterval(zoomAnimationInterval);

	connect(anim, SIGNAL(valueChanged(qreal)), this, slot);
	connect(anim, SIGNAL(finished()), this, SLOT(animFinished()));
	anim->start();
}

void EditorView::setDrawSceneGrid(bool show)
{
	mScene.setNeedDrawGrid(show);
	mScene.invalidate();
}

void EditorView::mouseMoveEvent(QMouseEvent *event)
{
	if (mWheelPressed) {
		if (mMouseOldPosition != QPointF()) {
			qreal const scaleFactor = transform().m11();
			qreal const dx = (event->localPos().x() - mMouseOldPosition.x()) / scaleFactor;
			qreal const dy = (event->localPos().y() - mMouseOldPosition.y()) / scaleFactor;
			viewport()->scroll(dx, dy);
			scene()->update();
		}

		mMouseOldPosition = event->localPos();
	}

	QGraphicsView::mouseMoveEvent(event);
	if (event->buttons() & Qt::RightButton) {
		setDragMode(NoDrag);
	} else {
		if ((event->buttons() & Qt::LeftButton) && (event->modifiers() & Qt::ControlModifier)) {
			setDragMode(RubberBandDrag);
			mScene.itemSelectUpdate();
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
}

void EditorView::mouseReleaseEvent(QMouseEvent *event)
{
	if (!(event->buttons() & Qt::MidButton)) {
		mWheelPressed = false;
		mMouseOldPosition = QPointF();
	}
	QGraphicsView::mouseReleaseEvent(event);
}

void EditorView::mousePressEvent(QMouseEvent *event)
{
	mWheelPressed = (event->buttons() & Qt::MidButton);
	mMouseOldPosition = QPointF();
	if (!mWheelPressed) {
		QGraphicsView::mousePressEvent(event);
	}

	if (event->buttons() == Qt::RightButton) {
		setDragMode(NoDrag);
	}

	if (event->buttons() & Qt::LeftButton) {
		if (!(event->buttons() & Qt::RightButton) && !mTouchManager.isGestureRunning()
				&& !itemAt(event->pos())) {
			setDragMode(RubberBandDrag);
		}
		if (event->modifiers() & Qt::ControlModifier) {
			mScene.itemSelectUpdate();
		}
	}
}

void EditorView::scrollContentsBy(int dx, int dy)
{
	QGraphicsView::scrollContentsBy(dx, dy);
	mScene.invalidate();
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

void EditorView::zoomInTime()
{
	qreal const zoomFactor = SettingsManager::value("zoomFactor").toReal();
	zoom(zoomFactor);
}

void EditorView::zoomOutTime()
{
	qreal const zoomFactor = 1 / SettingsManager::value("zoomFactor").toReal();
	zoom(zoomFactor);
}

void EditorView::animFinished()
{
	delete sender();
}

void EditorView::zoom(qreal const zoomFactor)
{
	qreal const oldScale = transform().m11();
	qreal const maxScale = SettingsManager::value("maxZoom").toReal();
	qreal const minScale = SettingsManager::value("minZoom").toReal();
	if ((zoomFactor > 1 && mathUtils::Math::geq(oldScale, maxScale)) ||
			(zoomFactor < 1 && mathUtils::Math::leq(oldScale, minScale))) {
		return;
	}

	setSceneRect(mScene.sceneRect());
	scale(zoomFactor, zoomFactor);

	if (SettingsManager::value("ShowGrid").toBool()) {
		mScene.setRealIndexGrid(mScene.realIndexGrid() * zoomFactor);
	}

	mMVIface.invalidateImagesZoomCache(transform().m11());

	checkGrid();
}

void EditorView::setSceneFont()
{
	if (SettingsManager::value("CustomFont").toBool()) {
		QFont font;
		font.fromString(SettingsManager::value("CurrentFont").toString());
		scene()->setFont(font);
		scene()->update();
	} else {
		/// @todo: use brand manager here
		scene()->setFont(QFont(QFontDatabase::applicationFontFamilies(
				QFontDatabase::addApplicationFont(QDir::currentPath() + "/times.ttf")).at(0), 9));
		scene()->update();
	}
}
