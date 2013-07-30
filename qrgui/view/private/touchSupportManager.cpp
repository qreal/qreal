#include "touchSupportManager.h"

#include <QtWidgets/QPinchGesture>

#include "../editorView.h"

using namespace qReal::view::details;

TouchSupportManager::TouchSupportManager(EditorView *editorView)
	: mEditorView(editorView)
	, mGestureIsRunning(false)
{
	mEditorView->setAttribute(Qt::WA_AcceptTouchEvents);
	mEditorView->grabGesture(Qt::PanGesture);
	mEditorView->grabGesture(Qt::TapGesture);
	mEditorView->grabGesture(Qt::PinchGesture);

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

	QMouseEvent* const mouseEvent = static_cast<QMouseEvent*>(event);

	switch (eventType) {
		case QEvent::Gesture: {
			QGestureEvent *gestureEvent = static_cast<QGestureEvent*>(event);

			if (QGesture *tap = gestureEvent->gesture(Qt::TapGesture)) {
				processGestureState(tap, false);
				mScroller.onTap();
			} else if (QGesture *pan = gestureEvent->gesture(Qt::PanGesture)) {
				mScroller.onPan(pan);
			} else if (QGesture *pinch = gestureEvent->gesture(Qt::PinchGesture)) {
				processGestureState(pinch);
				QPinchGesture *pinchGesture = static_cast<QPinchGesture *>(pinch);
				mEditorView->zoom(pinchGesture->scaleFactor());
			}

			return true;
		}
		case QEvent::MouseButtonPress:
			mScroller.onMousePress(mouseEvent);
			break;
		case QEvent::MouseMove:
			mScroller.onMouseMove(mouseEvent);
			break;
		case QEvent::MouseButtonRelease:
			mScroller.onMouseRelease(object, mouseEvent);
			break;
		default:
			break;
	}

	return false; // filter event
}

void TouchSupportManager::processGestureState(QGesture const *gesture, bool processFinished)
{
	if (gesture->state() == Qt::GestureStarted) {
		mGestureIsRunning = true;
		mEditorView->setDragMode(QGraphicsView::NoDrag);
		emit gestureStarted();
	} else if (gesture->state() == Qt::GestureFinished && processFinished) {
		mEditorView->setDragMode(QGraphicsView::RubberBandDrag);
		mGestureIsRunning = false;
		emit gestureFinished();
	}
}
