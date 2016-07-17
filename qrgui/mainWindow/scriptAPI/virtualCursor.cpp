/* Copyright 2014-2015 Dmitry Chernov, Dmitry Mordvinov
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

#include "virtualCursor.h"

#include <QtCore/QPropertyAnimation>
#include <QtCore/QTimer>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QApplication>

#include "mainWindow.h"
#include "palette/draggableElement.h"
#include "scriptAPI.h"

using namespace qReal;
using namespace gui;

int const cursorSize = 32;
int const dragIconSize = 32;

VirtualCursor::VirtualCursor(ScriptAPI &scriptAPI, QWidget *parent)
	: QLabel(parent)
	, mScriptAPI(scriptAPI)
	, mCursorMoveAnimation(new QPropertyAnimation(this, "geometry"))
	, mCursorPixmap(":/mainWindow/images/scriptAPI/virtcursor.png")
	, mRightButtonPressed(false)
	, mDragStarted(false)
{
	setFixedSize(cursorSize, cursorSize);
	setWindowFlags(Qt::WindowStaysOnTopHint);

	connect(mCursorMoveAnimation, &QPropertyAnimation::finished, &mScriptAPI, &ScriptAPI::breakWaiting);
}

void VirtualCursor::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event)

	QPainter painter(this);
	painter.drawPixmap(0, 0, cursorSize, cursorSize, mCursorPixmap);

	if (mDragStarted) {
		setFixedSize(cursorSize + dragIconSize, cursorSize + dragIconSize);
		painter.drawPixmap(cursorSize, cursorSize, dragIconSize, dragIconSize, mDragPixmap);
	} else {
		setFixedSize(cursorSize, cursorSize);
	}
}

void VirtualCursor::moveTo(const QWidget *target, int duration)
{
	if (target) {
		const int xcoord = target->mapTo(parentWidget(), QPoint(target->width() / 2, 0)).x();
		const int ycoord = target->mapTo(parentWidget(), QPoint(0, target->height() / 2)).y();

		animate(mapToParent(QPoint()), xcoord, ycoord, duration);
	}
}

void VirtualCursor::moveToRect(const QRect &target, int duration)
{
	animate(pos(), target.topLeft(), duration);
}

void VirtualCursor::moveToPoint(int x, int y, int duration)
{
	animate(mapToParent(QPoint()), x, y, duration);
}

void VirtualCursor::sceneMoveTo(QWidget *target, int duration, int xSceneCoord, int ySceneCoord)
{
	if (target) {
		// Adding 20 to coordinates needed to navigate to the center of targeted item because it doesn`t have size.
		const int xcoord = target->mapTo(parentWidget(), QPoint(0, 0)).x() + xSceneCoord + 20;
		const int ycoord = target->mapTo(parentWidget(), QPoint(0, 0)).y() + ySceneCoord + 20;

		QTimer * const timer = new QTimer(this);
		if (mRightButtonPressed) {
			timer->setInterval(100);

			// Sending move events for every 100 ms for drawing gestures.
			connect(timer, &QTimer::timeout
					, [this, target]() {
						const QPoint cursorPos = target->mapFrom(parentWidget(), pos());
						simulateMouse(target, QEvent::MouseMove, cursorPos, Qt::NoButton);
					});
			timer->start();
		}

		animate(pos(), xcoord, ycoord, duration);

		timer->stop();
	}
}

void VirtualCursor::leftButtonPress(QWidget *target, int delay)
{
	simulateMouse(target, QEvent::MouseButtonPress, widgetPos(target), Qt::LeftButton);

	if (delay >= 0) {
		mScriptAPI.wait(delay);
	}
}

void VirtualCursor::leftButtonRelease(QWidget *target, int delay)
{
	simulateMouse(target, QEvent::MouseButtonRelease, widgetPos(target), Qt::LeftButton);

	if (delay >= 0) {
		mScriptAPI.wait(delay);
	}
}

void VirtualCursor::rightButtonPress(QWidget *target, int delay)
{
	mRightButtonPressed = true;
	simulateMouse(target, QEvent::MouseButtonPress, widgetPos(target), Qt::RightButton);

	if (delay >= 0) {
		mScriptAPI.wait(delay);
	}
}

void VirtualCursor::rightButtonRelease(QWidget *target, int delay)
{
	mRightButtonPressed = false;
	simulateMouse(target, QEvent::MouseButtonRelease, widgetPos(target), Qt::RightButton);

	if (delay >= 0) {
		mScriptAPI.wait(delay);
	}
}

void VirtualCursor::startDrag(QIcon const &icon)
{
	mDragStarted = true;
	mDragPixmap = icon.pixmap(dragIconSize, dragIconSize);
	update();
}

void VirtualCursor::endDrag()
{
	 mDragStarted = false;
	 update();
}

void VirtualCursor::moved(QWidget *target)
{
	simulateMouse(target, QEvent::MouseMove, widgetPos(target), Qt::NoButton);
}

void VirtualCursor::simulateMouse(QObject *reciever, QEvent::Type event, QPointF const &pos
		, Qt::MouseButton button)
{
	QMouseEvent *mouseEvent = new QMouseEvent(event, pos, button, button, Qt::NoModifier);
	QApplication::postEvent(reciever, mouseEvent);
}

QPoint VirtualCursor::widgetPos(QWidget *target) const
{
	return target->mapFrom(parentWidget(), pos());
}

void VirtualCursor::animate(const QPoint &from, int toX, int toY, int duration)
{
	animate(from, QPoint(toX, toY), duration);
}

void VirtualCursor::animate(const QPoint &from, const QPoint &to, int duration)
{
	mCursorMoveAnimation->setDuration(duration);
	mCursorMoveAnimation->setStartValue(QRect(from, QSize()));
	mCursorMoveAnimation->setEndValue(QRect(to, QSize()));
	mCursorMoveAnimation->start();
	// To make it synchronious. The waiting will be broken by connection made in constructor.
	mScriptAPI.wait(-1);
}
