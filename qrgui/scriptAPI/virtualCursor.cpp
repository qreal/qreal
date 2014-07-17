#include "virtualCursor.h"

#include <QtTest/QTest>

#include "scriptAPI.h"
#include "mainwindow/mainWindow.h"
#include "mainwindow/palette/draggableElement.h"

using namespace qReal;
using namespace gui;

VirtualCursor::VirtualCursor(ScriptAPI *scriptAPI, QWidget *parent)
	: QLabel(parent)
	, mScriptAPI(scriptAPI)
	, mCursorMoveAnimation (new QPropertyAnimation(this, "geometry"))
	, mRightButtonPressed(false)
	, mIsPaletteElementAttached(false)
{
	setFixedSize(32, 32);
	show();
	setWindowFlags(Qt::WindowStaysOnTopHint);
	raise();
}

void VirtualCursor::paintEvent(QPaintEvent *event)
{
	(void)event;
	QPainter mPainter(this);
	QPixmap const virtCursorIcon(":/icons/virtcursor.png");
	mPainter.drawPixmap(0,0, virtCursorIcon);
	if (mIsPaletteElementAttached) {
		setFixedSize(64, 64);
		mPainter.drawPixmap(QPoint(32,32), mPaletteElementIcon.pixmap(32, 32));
	} else {
		setFixedSize(32, 32);
	}
}

void VirtualCursor::moveTo(QWidget const *target, int duration)
{
	int const xcoord = target->mapTo(parentWidget(), QPoint()).x();
	int const ycoord = target->mapTo(parentWidget(), QPoint()).y();

	mCursorMoveAnimation->setDuration(duration);
	mCursorMoveAnimation->setStartValue(QRect(mapToParent(QPoint()).x(), mapToParent(QPoint()).y(), 0, 0));
	mCursorMoveAnimation->setEndValue(QRect(xcoord, ycoord, 0, 0));

	connect (mCursorMoveAnimation, &QPropertyAnimation::finished, mScriptAPI, &ScriptAPI::breakWaiting);

	mCursorMoveAnimation->start();

	mScriptAPI->wait(-1);
}

void VirtualCursor::moveToRect(QRect const &target, int duration)
{
	int const xcoord = target.topLeft().x();
	int const ycoord = target.topLeft().y();

	mCursorMoveAnimation->setDuration(duration);
	mCursorMoveAnimation->setStartValue(QRect(pos().x(), pos().y(), 0, 0));
	mCursorMoveAnimation->setEndValue(QRect(xcoord, ycoord, 0, 0));

	connect (mCursorMoveAnimation, &QPropertyAnimation::finished, mScriptAPI, &ScriptAPI::breakWaiting);

	mCursorMoveAnimation->start();

	mScriptAPI->wait(-1);
}

void VirtualCursor::sceneMoveTo(QWidget *target, int duration, int xSceneCoord, int ySceneCoord)
{
	int const xcoord = target->mapToGlobal(QPoint(0, 0)).x() + xSceneCoord;
	int const ycoord = target->mapToGlobal(QPoint(0, 0)).y() + ySceneCoord;

	mCursorMoveAnimation->setDuration(duration);
	mCursorMoveAnimation->setStartValue(QRect(pos().x(), pos().y(), 0, 0));
	mCursorMoveAnimation->setEndValue(QRect(xcoord, ycoord, 0, 0));

	connect (mCursorMoveAnimation, &QPropertyAnimation::finished, mScriptAPI, &ScriptAPI::breakWaiting);

	if (mRightButtonPressed) {
		QTimer *timer = new QTimer(this);
		timer->setInterval(100);

		connect(timer, &QTimer::timeout,
			[this, target]() {
				QEvent *moveEvent = new QMouseEvent(QMouseEvent::MouseMove
													, target->mapFromGlobal(pos())
													, Qt::RightButton
													, Qt::RightButton
													, Qt::NoModifier);
				QApplication::postEvent(target, moveEvent);
			});
		timer->start();
	}

	mCursorMoveAnimation->start();

	mScriptAPI->wait(-1);
}

void VirtualCursor::leftButtonPress(QWidget *target, int delay)
{
	QPoint cursorPos = target->mapFrom(parentWidget(), pos());
	QEvent *pressEvent = new QMouseEvent(QMouseEvent::MouseButtonPress
										 , cursorPos
										 , Qt::LeftButton
										 , Qt::LeftButton
										 , Qt::NoModifier);
	QApplication::postEvent(target, pressEvent);

	if (delay != -1) {
		mScriptAPI->wait(delay);
	}
}

void VirtualCursor::leftButtonRelease(QWidget *target, int delay)
{
	QPoint cursorPos = target->mapFrom(parentWidget(), pos());
	QEvent *releaseEvent = new QMouseEvent(QMouseEvent::MouseButtonRelease
										   , cursorPos
										   , Qt::LeftButton
										   , Qt::LeftButton
										   , Qt::NoModifier);
	QApplication::postEvent(target, releaseEvent);

	if (delay != -1) {
		mScriptAPI->wait(delay);
	}
}

void VirtualCursor::rightButtonPress(QWidget *target, int delay)
{
	mRightButtonPressed = true;
	QPoint cursorPos = target->mapFrom(parentWidget(), pos());
	QEvent *pressEvent = new QMouseEvent(QMouseEvent::MouseButtonPress
										 , cursorPos
										 , Qt::RightButton
										 , Qt::RightButton
										 , Qt::NoModifier);
	QApplication::postEvent(target, pressEvent);

	if (delay != -1) {
		mScriptAPI->wait(delay);
	}
}

void VirtualCursor::rightButtonRelease(QWidget  *target, int delay)
{
	mRightButtonPressed = false;
	QPoint cursorPos = target->mapFrom(parentWidget(), pos());
	QEvent *releaseEvent = new QMouseEvent(QMouseEvent::MouseButtonRelease
										   , cursorPos
										   , Qt::RightButton
										   , Qt::RightButton
										   , Qt::NoModifier);
	QApplication::postEvent(target, releaseEvent);

	if (delay != -1) {
		mScriptAPI->wait(delay);
	}
}

void VirtualCursor::attachPaletteElement(QIcon icon)
{
	mIsPaletteElementAttached = true;
	mPaletteElementIcon = icon;
	update();
}

void VirtualCursor::detachPaletteElementIcon()
{
	 mIsPaletteElementAttached = false;
	 update();
}

void VirtualCursor::moved(QWidget *target)
{
	QPoint cursorPos = target->mapFrom(parentWidget(), pos());
	QEvent *moveEvent = new QMouseEvent(QMouseEvent::MouseMove
										, cursorPos
										, Qt::RightButton
										, Qt::RightButton
										, Qt::NoModifier);
	QApplication::postEvent(target, moveEvent);
}
