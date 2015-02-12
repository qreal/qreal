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

// Customized with QReal-only needs

#include "qsKineticScroller.h"
#include <QApplication>
#include <QScrollBar>
#include <QAbstractScrollArea>
#include <QEvent>
#include <QTimer>
#include <QtCore/qmath.h>
#include <QtCore/QDebug>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QPanGesture>
#include <cstddef> // for nullptr

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
static const int gFriction = 4;

class QsKineticScrollerImpl
{
public:
   QsKineticScrollerImpl()
	  : scrollArea(nullptr)
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
   QPoint averageDelta;
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
	if (!scrollArea) {
		Q_ASSERT_X(0, "kinetic scroller", "missing scroll area");
		return;
	}
	d->scrollArea = scrollArea;
}

bool QsKineticScroller::shouldIgnoreMouseAction()
{
	return d->ignoredMouseActions-- > 0;
}

void QsKineticScroller::onTap()
{
	d->isMoving = false;
}

void QsKineticScroller::onPan(QGesture *pan)
{
	QPanGesture *panGesture = static_cast<QPanGesture *>(pan);
//	d->averageDelta = (2 * d->averageDelta + 3 * panGesture->delta().toPoint()) / 5;

	if (pan->state() == Qt::GestureFinished) {
//		QPoint offset(5 * d->averageDelta);

//		d->isPressed = false;
//		d->isMoving = true;
//		d->averageDelta = QPoint();

//		d->velocityY = (offset.y() + d->velocityY) / 2;
//		d->velocityX = (offset.x() + d->velocityX) / 2;

//		computeFriction();

//		if (!d->kineticTimer.isActive()) {
//			d->kineticTimer.start(gTimerInterval);
//		}
	} else {
		QVector2D offset(-panGesture->delta().toPoint());
		d->lastVerticalScrollBarPosition = d->scrollArea->verticalScrollBar()->value();
		d->lastHorizontalScrollBarPosition = d->scrollArea->horizontalScrollBar()->value();
		d->scrollArea->verticalScrollBar()->setValue(d->lastVerticalScrollBarPosition + offset.y());
		d->scrollArea->horizontalScrollBar()->setValue(d->lastHorizontalScrollBarPosition + offset.x());
	}
}

void QsKineticScroller::onMousePress(QMouseEvent *mouseEvent)
{
	if (mouseEvent->button() == Qt::MiddleButton) {
		d->isPressed = true;
		d->lastPressPoint = mouseEvent->pos();
		d->lastVerticalScrollBarPosition = d->scrollArea->verticalScrollBar()->value();
		d->lastHorizontalScrollBarPosition = d->scrollArea->horizontalScrollBar()->value();
		if (d->isMoving) { // press while kinetic scrolling, so stop
			d->stopMotion();
		}
	}
}

void QsKineticScroller::onMouseMove(QMouseEvent *mouseEvent)
{
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

//			if (!d->kineticTimer.isActive()) {
//				d->kineticTimer.start(gTimerInterval);
//			}
		}
	} else {
		// manual scroll
		if (d->isPressed) {
			const int dragDistanceY = mouseEvent->pos().y() - d->lastPressPoint.y();
			const int dragDistanceX = mouseEvent->pos().x() - d->lastPressPoint.x();
			d->scrollArea->verticalScrollBar()->setValue(d->lastVerticalScrollBarPosition - dragDistanceY);
			d->scrollArea->horizontalScrollBar()->setValue(d->lastHorizontalScrollBarPosition - dragDistanceX);
		}
	}
}

void QsKineticScroller::onMouseRelease(QObject *eventSource, QMouseEvent *mouseEvent)
{
	Q_UNUSED(eventSource)
	if (mouseEvent->button() == Qt::MiddleButton) {
		d->isPressed = false;
		// Looks like the user wanted a single click. Simulate the click,
		// as the events were already consumed
		// if (!d->isMoving) {
		//	QMouseEvent* mousePress = new QMouseEvent(QEvent::MouseButtonPress,
		//	   d->lastPressPoint, Qt::MiddleButton, Qt::MiddleButton, Qt::NoModifier);
		//	QMouseEvent* mouseRelease = new QMouseEvent(QEvent::MouseButtonRelease,
		//	   d->lastPressPoint, Qt::MiddleButton, Qt::MiddleButton, Qt::NoModifier);

		//	d->ignoredMouseActions = 2;
		//	QApplication::postEvent(eventSource, mousePress);
		//	QApplication::postEvent(eventSource, mouseRelease);
		//}
	}
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
	const qreal vlen = qSqrt(qPow(d->velocityX, 2) + qPow(d->velocityY, 2));
	qreal angle = 0;
	const qreal pi = 3.1415926535;

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

	const qreal flen = qSqrt(qPow(d->frictionX, 2) + qPow(d->frictionY, 2));

	d->frictionX = gFriction * d->frictionX / flen;
	d->frictionY = gFriction * d->frictionY / flen;

}
