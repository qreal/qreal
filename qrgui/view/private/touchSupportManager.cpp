#include "touchSupportManager.h"

#include <QtWidgets/QPinchGesture>

#include "../editorView.h"

using namespace qReal::view::details;

TouchSupportManager::TouchSupportManager(EditorView *editorView)
	: mEditorView(editorView)
	, mGestureIsRunning(false)
{
	mEditorView->grabGesture(Qt::PanGesture);
	mEditorView->grabGesture(Qt::TapGesture);
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
		case QEvent::Gesture: {
			QGestureEvent *gestureEvent = static_cast<QGestureEvent *>(event);

			if (gestureEvent->gesture(Qt::TapGesture)) {
				mScroller.onTap();
			} else if (QGesture *pan = gestureEvent->gesture(Qt::PanGesture)) {
				mScroller.onPan(pan);
			} else if (QGesture *pinch = gestureEvent->gesture(Qt::PinchGesture)) {
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
		// For some reason touch events can`t be processed here and must be
		// processed in scroll area`s viewport event
		default:
			break;
	}

	return false;
}

bool TouchSupportManager::processTouchEvent(QTouchEvent *event)
{
	mGestureIsRunning = event->type() != QEvent::TouchEnd
			&& event->type() != QEvent::TouchCancel;
	mEditorView->setDragMode(mGestureIsRunning
			? QGraphicsView::NoDrag
			: QGraphicsView::RubberBandDrag);
	return true;
}
