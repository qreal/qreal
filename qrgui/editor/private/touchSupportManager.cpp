/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "touchSupportManager.h"

#include <QtWidgets/QPinchGesture>
#include <QtWidgets/QApplication>

#include <qrutils/mathUtils/geometry.h>

#include "editor/editorView.h"

using namespace qReal::gui::editor::view::details;

TouchSupportManager::TouchSupportManager(EditorView *editorView)
	: mEditorView(editorView)
	, mGestureIsRunning(false)
	, mFingersInGesture(0)
{
	mEditorView->grabGesture(Qt::TapGesture);
	mEditorView->grabGesture(Qt::TapAndHoldGesture);
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
	const QEvent::Type eventType = event->type();
	const bool isMouseAction = QEvent::MouseButtonPress == eventType || QEvent::MouseButtonRelease == eventType;
	const bool isMouseEvent = isMouseAction || QEvent::MouseMove == eventType;

	if (!isMouseEvent && event->type() != QEvent::Gesture) {
		return false;
	}

	if (isMouseAction && mScroller.shouldIgnoreMouseAction()) { // don't filter simulated click
		return false;
	}

	QMouseEvent * const mouseEvent = dynamic_cast<QMouseEvent *>(event);
#if(QT_VERSION >= QT_VERSION_CHECK(5, 3, 0))
	if (isMouseEvent && mouseEvent->source() != Qt::MouseEventNotSynthesized) {
		// Starting from version 5.3.0 Qt generates extra mouse events even on
		// accepted touch events, filtering out such case...
		return true;
	}
#endif

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

void TouchSupportManager::grabTapAndHold()
{
	mEditorView->grabGesture(Qt::TapAndHoldGesture);
}

void TouchSupportManager::simulateMouse(QObject *reciever, QEvent::Type event, const QPointF &pos
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

void TouchSupportManager::simulateRightClick(QTapAndHoldGesture *gesture)
{
	const QPointF position(mEditorView->viewport()->mapFromGlobal(gesture->position().toPoint()));

	mButton = Qt::LeftButton;
	simulateMouse(mEditorView->viewport(), QEvent::MouseButtonPress, position, Qt::LeftButton);
	simulateMouse(mEditorView->viewport(), QEvent::MouseButtonRelease, position, Qt::NoButton);

	mButton = Qt::RightButton;
	simulateMouse(mEditorView->viewport(), QEvent::MouseButtonPress, position, Qt::RightButton);
	simulateMouse(mEditorView->viewport(), QEvent::MouseButtonRelease, position, Qt::NoButton);
}

bool TouchSupportManager::isElementUnder(const QPointF &pos)
{
	for (QGraphicsItem * const item : mEditorView->items(pos.toPoint())) {
		if (dynamic_cast<Element *>(item)) {
			return true;
		}
	}

	return false;
}

bool TouchSupportManager::handleGesture(QGestureEvent *gestureEvent)
{
	if (gestureEvent->gesture(Qt::TapGesture)) {
		mScroller.onTap();
	} else if (QGesture *tapAndHold = gestureEvent->gesture(Qt::TapAndHoldGesture)) {
		// Filters out tap & hold with mouse
		if (mFingersInGesture > 0) {
			processGestureState(tapAndHold);
			simulateRightClick(static_cast<QTapAndHoldGesture *>(tapAndHold));
		}
	} else if (QGesture *pan = gestureEvent->gesture(Qt::PanGesture)) {
		processGestureState(pan);
		if (mFingersInGesture > 2) {
			mScroller.onPan(pan);
		}
	} else if (QGesture *pinch = gestureEvent->gesture(Qt::PinchGesture)) {
		processGestureState(pinch);
		QPinchGesture *pinchGesture = static_cast<QPinchGesture *>(pinch);
		mEditorView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
		mEditorView->zoom(pinchGesture->scaleFactor());
		mEditorView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
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
	if (event->device()->type() == 1) {
		return false;
	}

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
	const QPointF touchPoint = event->touchPoints()[0].pos();
	switch(event->type()) {
	case QEvent::TouchBegin: {
		mEditorView->scene()->clearSelection();
		const bool elementUnder = isElementUnder(event->touchPoints()[0].pos());
		moveCursor(event);
		if (QDateTime::currentMSecsSinceEpoch() - mLastTapTimestamp <= QApplication::doubleClickInterval()) {
			// Double tap occured. We don`t want to show context menu after double tap so disabling
			// corresponding gesture event with enabling it later
			mEditorView->ungrabGesture(Qt::TapAndHoldGesture);

			if (elementUnder) {
				// Simulating regular left button click
				simulatePress(event);
			} else {
				// Simulating double-click
				simulateDoubleClick(event);
			}

			// For some reason grabbing tap & hold back right now still generates gesture event
			QTimer::singleShot(30, this, SLOT(grabTapAndHold()));
		} else {
			if (elementUnder) {
				// Simulating right button click for links gesture
				simulatePress(event, Qt::RightButton);
			} else {
				// Simulating right button click for mouse gestures drawing or left button click
				// for clearing selection
				simulatePress(event, mEditorView->scene()->selectedItems().isEmpty()
						? Qt::RightButton : Qt::LeftButton);
			}
		}

		mEditorView->scene()->update();
		mLastTapTimestamp = QDateTime::currentMSecsSinceEpoch();
		mLastTouchBeginPoint = touchPoint;
		break;
	}
	case QEvent::TouchEnd: {
		// This will not show context menu when we just tap on scene
		if (mButton == Qt::RightButton) {
			simulateMove(event);
		}

		simulateRelease(event);

		// If user`s touch begin and end events points distinguish not more than this distance then the element
		// under this point will be selected.
		const qreal maxDistance = 10;
		if (isElementUnder(touchPoint)
				&& mathUtils::Geometry::distance(mLastTouchBeginPoint, touchPoint) < maxDistance) {
			// Selecting the element under the finger
			simulatePress(event);
			simulateRelease(event);
		}

		mEditorView->scene()->update();
		break;
	}
	case QEvent::TouchUpdate:
		simulateMove(event);
		break;
	default:
		break;
	}
}

void TouchSupportManager::moveCursor(QTouchEvent *event)
{
	QCursor::setPos(mEditorView->viewport()->mapToGlobal(event->touchPoints()[0].pos().toPoint()));
}
