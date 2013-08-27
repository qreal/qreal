#include "touchSupportManager.h"

#include <QtWidgets/QPinchGesture>
#include <QtWidgets/QApplication>

#include "view/editorView.h"

using namespace qReal::view::details;

TouchSupportManager::TouchSupportManager(EditorView *editorView)
	: mEditorView(editorView)
	, mGestureIsRunning(false)
{
	mEditorView->grabGesture(Qt::TapGesture);
	mEditorView->grabGesture(Qt::PanGesture);
	mEditorView->grabGesture(Qt::PinchGesture);
	mEditorView->viewport()->setAttribute(Qt::WA_AcceptTouchEvents);

	mEditorView->installEventFilter(this);
	mEditorView->viewport()->installEventFilter(this);

	mScroller.enableKineticScrollFor(mEditorView);
}

bool TouchSupportManager::isGestureRunning() const
{
	return mGestureIsRunning;
}

bool TouchSupportManager::eventFilter(QObject *object, QEvent *event)
{
	QEvent::Type const eventType = event->type();
	bool const isMouseAction = QEvent::MouseButtonPress == eventType || QEvent::MouseButtonRelease == eventType;
	bool const isMouseEvent = isMouseAction || QEvent::MouseMove == eventType;

	if (!isMouseEvent && event->type() != QEvent::Gesture) {
		return false;
	}

	if (isMouseAction && mScroller.shouldIgnoreMouseAction()) { // don't filter simulated click
		return false;
	}

	QMouseEvent * const mouseEvent = static_cast<QMouseEvent *>(event);

	switch (eventType) {
		case QEvent::Gesture:
			return handleGesture(static_cast<QGestureEvent *>(event));
		case QEvent::MouseButtonPress:
			mScroller.onMousePress(mouseEvent);
			break;
		case QEvent::MouseMove:
			mScroller.onMouseMove(mouseEvent);
			break;
		case QEvent::MouseButtonRelease:
			mScroller.onMouseRelease(object, mouseEvent);
			break;
		// For some reason touch events can`t be processed here and must be
		// processed in scroll area`s viewport event
		default:
			break;
	}

	return false;
}

void TouchSupportManager::simulateMouse(QObject *reciever, QEvent::Type event, QPointF const &pos
		, Qt::MouseButtons buttons)
{
	QMouseEvent *mouseEvent = new QMouseEvent(event, pos, mButton, buttons, Qt::NoModifier);
	QApplication::postEvent(reciever, mouseEvent);
}

void TouchSupportManager::simulatePress(QTouchEvent *event, Qt::MouseButton button)
{
	mButton = button;
	simulateMouse(event->target(), QEvent::MouseButtonPress, event->touchPoints()[0].pos(), mButton);
}

void TouchSupportManager::simulateMove(QTouchEvent *event)
{
	simulateMouse(event->target(), QEvent::MouseMove, event->touchPoints()[0].pos(), mButton);
}

void TouchSupportManager::simulateRelease(QTouchEvent *event)
{
	simulateMouse(event->target(), QEvent::MouseButtonRelease, event->touchPoints()[0].pos(), Qt::NoButton);
}

void TouchSupportManager::simulateDoubleClick(QTouchEvent *event)
{
	mButton = Qt::LeftButton;
	simulateMouse(event->target(), QEvent::MouseButtonDblClick, event->touchPoints()[0].pos(), mButton);
}

bool TouchSupportManager::isElementUnder(QPointF const &pos)
{
	return mEditorView->itemAt(pos.toPoint());
}

bool TouchSupportManager::handleGesture(QGestureEvent *gestureEvent)
{
	if (gestureEvent->gesture(Qt::TapGesture)) {
		mScroller.onTap();
	} else if (QGesture *pan = gestureEvent->gesture(Qt::PanGesture)) {
		processGestureState(pan);
		mScroller.onPan(pan);
	} else if (QGesture *pinch = gestureEvent->gesture(Qt::PinchGesture)) {
		processGestureState(pinch);
		QPinchGesture *pinchGesture = static_cast<QPinchGesture *>(pinch);
		mEditorView->zoom(pinchGesture->scaleFactor());
	}
	return true;
}

void TouchSupportManager::processGestureState(QGesture *gesture)
{
	if (gesture->state() == Qt::GestureStarted) {
		emit gestureStarted();
	} else if (gesture->state() == Qt::GestureFinished) {
		emit gestureFinished();
	}
}

bool TouchSupportManager::processTouchEvent(QTouchEvent *event)
{
	event->accept();
	mGestureIsRunning = event->type() != QEvent::TouchEnd;
	mEditorView->setDragMode(QGraphicsView::NoDrag);
	mFingersInGesture = event->touchPoints().count();
	if (mFingersInGesture == 1) {
		handleOneFingerTouch(event);
	}
	return true;
}

void TouchSupportManager::handleOneFingerTouch(QTouchEvent *event)
{
	switch(event->type()) {
	case QEvent::TouchBegin: {
		mEditorView->scene()->clearSelection();
		bool const elementUnder = isElementUnder(event->touchPoints()[0].pos());

		if (QDateTime::currentMSecsSinceEpoch() - mLastTapTimestamp <= QApplication::doubleClickInterval()) {
			if (elementUnder) {
				// Simulating right button click for links gesture
				simulatePress(event, Qt::RightButton);
			} else {
				// Simulating double-click
				simulateDoubleClick(event);
			}
		} else {
			if (elementUnder) {
				// Simulating regular left button click
				simulatePress(event);
			} else {
				// Simulating right button click for mouse gestures drawing or left button click
				// for clearing selection
				simulatePress(event, mEditorView->scene()->selectedItems().isEmpty()
						? Qt::RightButton : Qt::LeftButton);
			}
		}

		mEditorView->scene()->update();
		mLastTapTimestamp = QDateTime::currentMSecsSinceEpoch();
		break;
	}
	case QEvent::TouchEnd:
		// This will not show context menu when we just tap on scene
		if (mButton == Qt::RightButton) {
			simulateMove(event);
		}

		simulateRelease(event);
		mEditorView->scene()->update();
		break;
	case QEvent::TouchUpdate:
		simulateMove(event);
		break;
	default:
		break;
	}
}
