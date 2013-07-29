// Copyright (c) 2010, Razvan Petru
// All rights reserved.

// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:

// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice, this
//   list of conditions and the following disclaimer in the documentation and/or other
//   materials provided with the distribution.
// * The name of the contributors may not be used to endorse or promote products
//   derived from this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.

#include "QsKineticScroller.h"
#include <QApplication>
#include <QScrollBar>
#include <QAbstractScrollArea>
#include <QMouseEvent>
#include <QEvent>
#include <QTimer>
#include <QtCore/qmath.h>
#include <QtCore/QDebug>
#include <QtWidgets/QGesture>
#include <QtWidgets/QGraphicsView>
#include <cstddef> // for NULL

// A number of mouse moves are ignored after a press to differentiate
// it from a press & drag.
static const int gMaxIgnoredMouseMoves = 4;
// The timer measures the drag speed & handles kinetic scrolling. Adjusting
// the timer interval will change the scrolling speed and smoothness.
static const int gTimerInterval = 50;
// The speed measurement is imprecise, limit it so that the scrolling is not
// too fast.
static const int gMaxDecelerationSpeed = 100;
// influences how fast the scroller decelerates
static const int gFriction = 5;

class QsKineticScrollerImpl
{
public:
   QsKineticScrollerImpl()
      : scrollArea(NULL)
      , isPressed(false)
      , isMoving(false)
      , lastMouseYPos(0)
	  , lastVerticalScrollBarPosition(0)
	  , lastHorizontalScrollBarPosition(0)
	  , velocityY(0)
	  , velocityX(0)
	  , frictionX(0)
	  , frictionY(0)
      , ignoredMouseMoves(0)
      , ignoredMouseActions(0) {}

   void stopMotion()
   {
      isMoving = false;
	  velocityX = 0;
	  velocityY = 0;
      kineticTimer.stop();
   }

   QAbstractScrollArea* scrollArea;
   bool isPressed;
   bool isMoving;
   QPoint lastPressPoint;
   int lastMouseYPos;
   int lastMouseXPos;
   int lastVerticalScrollBarPosition;
   int lastHorizontalScrollBarPosition;
   int velocityY;
   int velocityX;
   int frictionX;
   int frictionY;
   int ignoredMouseMoves;
   int ignoredMouseActions;
   QTimer kineticTimer;
};

QsKineticScroller::QsKineticScroller(QObject *parent)
   : QObject(parent)
   , d(new QsKineticScrollerImpl)
{
   connect(&d->kineticTimer, SIGNAL(timeout()), SLOT(onKineticTimerElapsed()));
}

// needed by smart pointer
QsKineticScroller::~QsKineticScroller()
{
}

void QsKineticScroller::enableKineticScrollFor(QAbstractScrollArea* scrollArea)
{
	if( !scrollArea ){
		Q_ASSERT_X(0, "kinetic scroller", "missing scroll area");
		return;
	}

	// remove existing association
	if( d->scrollArea ) {
		d->scrollArea->viewport()->removeEventFilter(this);
		d->scrollArea->removeEventFilter(this);
		d->scrollArea = NULL;
	}

	// associate
	scrollArea->installEventFilter(this);
	scrollArea->viewport()->installEventFilter(this);
	d->scrollArea = scrollArea;
}

//! intercepts mouse events to make the scrolling work
bool QsKineticScroller::eventFilter(QObject* object, QEvent* event)
{
	const QEvent::Type eventType = event->type();
	const bool isMouseAction = QEvent::MouseButtonPress == eventType || QEvent::MouseButtonRelease == eventType;
	const bool isMouseEvent = isMouseAction || QEvent::MouseMove == eventType;

	if ((!isMouseEvent && event->type() != QEvent::Gesture) || !d->scrollArea) {
		return false;
	}

	if (isMouseAction && d->ignoredMouseActions-- > 0) { // don't filter simulated click
		return false;
	}

	QMouseEvent* const mouseEvent = static_cast<QMouseEvent*>(event);

	switch (eventType) {
		case QEvent::Gesture: {
			QGestureEvent *gestureEvent = static_cast<QGestureEvent*>(event);

			if (gestureEvent->gesture(Qt::TapGesture)) {
				d->isMoving = false;
			}

			if (QGesture *pan = gestureEvent->gesture(Qt::PanGesture)) {
				QPanGesture *panGesture = static_cast<QPanGesture *>(pan);
				QVector2D offset(panGesture->lastOffset().toPoint());

				d->isPressed = false;
				d->isMoving = true;

				d->velocityY = offset.y();
				d->velocityX = offset.x();

				computeFriction();

				if (!d->kineticTimer.isActive()) {
					d->kineticTimer.start(gTimerInterval);
				}
			}

			break;
		}
		case QEvent::MouseButtonPress:{
			if (mouseEvent->button() == Qt::MiddleButton) {
				d->isPressed = true;
				d->lastPressPoint = mouseEvent->pos();
				d->lastVerticalScrollBarPosition = d->scrollArea->verticalScrollBar()->value();
				d->lastHorizontalScrollBarPosition = d->scrollArea->horizontalScrollBar()->value();
				if (d->isMoving) { // press while kinetic scrolling, so stop
					d->stopMotion();
				}
			}
			break;
		}
		case QEvent::MouseMove:{
			if (!d->isMoving){
				// A few move events are ignored as "click jitter", but after that we
				// assume that the user is doing a click & drag
				if (d->ignoredMouseMoves < gMaxIgnoredMouseMoves) {
				   ++d->ignoredMouseMoves;
				} else {
					d->ignoredMouseMoves = 0;
					d->isMoving = true;
					d->lastMouseYPos = mouseEvent->pos().y();
					d->lastMouseXPos = mouseEvent->pos().x();

					if (!d->kineticTimer.isActive()) {
						d->kineticTimer.start(gTimerInterval);
					}
				}
			} else {
				// manual scroll
				if (d->isPressed) {
					const int dragDistanceY = mouseEvent->pos().y() - d->lastPressPoint.y();
					const int dragDistanceX = mouseEvent->pos().x() - d->lastPressPoint.x();
					d->scrollArea->verticalScrollBar()->setValue(d->lastVerticalScrollBarPosition - dragDistanceY);
					d->scrollArea->horizontalScrollBar()->setValue(d->lastHorizontalScrollBarPosition - dragDistanceX);
				}
				return false;
			}
			break;
		}
		case QEvent::MouseButtonRelease: {
			if (mouseEvent->button() == Qt::MiddleButton) {
				d->isPressed = false;
				// Looks like the user wanted a single click. Simulate the click,
				// as the events were already consumed
				if (!d->isMoving) {
					QMouseEvent* mousePress = new QMouseEvent(QEvent::MouseButtonPress,
					   d->lastPressPoint, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
					QMouseEvent* mouseRelease = new QMouseEvent(QEvent::MouseButtonRelease,
					   d->lastPressPoint, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);

					d->ignoredMouseActions = 2;
					QApplication::postEvent(object, mousePress);
					QApplication::postEvent(object, mouseRelease);
				}
			}
			break;
		}
		default:
		  break;
	}

	return true; // filter event
}

void QsKineticScroller::onKineticTimerElapsed()
{
	if (d->isPressed && d->isMoving) {
		// the speed is measured between two timer ticks
		const int cursorYPos = d->scrollArea->mapFromGlobal(QCursor::pos()).y();
		const int cursorXPos = d->scrollArea->mapFromGlobal(QCursor::pos()).x();

		d->velocityY = cursorYPos - d->lastMouseYPos;
		d->velocityX = cursorXPos - d->lastMouseXPos;

		d->lastMouseYPos = cursorYPos;
		d->lastMouseXPos = cursorXPos;

		computeFriction();
	} else {
		if( !d->isPressed && d->isMoving ) {
			// use the previously recorded speed and gradually decelerate
			d->velocityY = qBound(-gMaxDecelerationSpeed, d->velocityY, gMaxDecelerationSpeed);
			if( d->velocityY > 0 )
			 d->velocityY -= d->frictionY;
			else if( d->velocityY < 0 )
			 d->velocityY += d->frictionY;
			if( qAbs(d->velocityY) < qAbs(d->frictionY) )
			 d->stopMotion();

			const int scrollBarYPos = d->scrollArea->verticalScrollBar()->value();
			d->scrollArea->verticalScrollBar()->setValue(scrollBarYPos - d->velocityY);
			//
			d->velocityX = qBound(-gMaxDecelerationSpeed, d->velocityX, gMaxDecelerationSpeed);
			if( d->velocityX > 0 )
			 d->velocityX -= d->frictionX;
			else if( d->velocityX < 0 )
			 d->velocityX += d->frictionX;
			if( qAbs(d->velocityX) < qAbs(d->frictionX) )
			 d->stopMotion();
			const int scrollBarXPos = d->scrollArea->horizontalScrollBar()->value();
			d->scrollArea->horizontalScrollBar()->setValue(scrollBarXPos - d->velocityX);
		} else {
			d->stopMotion();
		}
	}
}

void QsKineticScroller::computeFriction()
{
	qreal const vlen = qSqrt(qPow(d->velocityX, 2) + qPow(d->velocityY, 2));
	qreal angle = 0;
	qreal const pi = 3.1415926535;

	if (d->velocityX == 0) {
		angle = pi / 2;
	} else {
		if (d->velocityY == 0) {
			angle = 0;
		} else {
			angle = qAcos(qAbs(d->velocityX / vlen));
		}
	}
	d->frictionX = qAbs(qRound(qCos(angle)));
	d->frictionY = qAbs(qRound(qSin(angle)));

	qreal const flen = qSqrt(qPow(d->frictionX, 2) + qPow(d->frictionY, 2));

	d->frictionX = gFriction * d->frictionX / flen;
	d->frictionY = gFriction * d->frictionY / flen;

}
