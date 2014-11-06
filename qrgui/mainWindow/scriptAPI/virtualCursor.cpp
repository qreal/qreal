#include "virtualCursor.h"

#include <QtWidgets/QApplication>

#include "mainwindow/mainWindow.h"
#include "mainwindow/palette/draggableElement.h"
#include "scriptAPI.h"

using namespace qReal;
using namespace gui;

VirtualCursor::VirtualCursor(ScriptAPI *scriptAPI, QWidget *parent)
	: QLabel(parent)
	, mScriptAPI(scriptAPI)
	, mCursorMoveAnimation(new QPropertyAnimation(this, "geometry"))
	, mRightButtonPressed(false)
	, mDragStarted(false)
{
	setFixedSize(32, 32);
	setWindowFlags(Qt::WindowStaysOnTopHint);
}

void VirtualCursor::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event)
	QPainter mPainter(this);
	QPixmap const virtCursorIcon(":/mainWindow/images/scriptAPI/virtcursor.png");
	mPainter.drawPixmap(0,0, virtCursorIcon);
	if (mDragStarted) {
		setFixedSize(64, 64);
		mPainter.drawPixmap(QPoint(32,32), mPaletteElementIcon.pixmap(32, 32));
	} else {
		setFixedSize(32, 32);
	}
}

void VirtualCursor::moveTo(QWidget const *target, int duration)
{
	if (target) {
		int const xcoord = target->mapTo(parentWidget(), QPoint(target->width()/2,0)).x();
		int const ycoord = target->mapTo(parentWidget(), QPoint(0, target->height()/2)	).y();

		mCursorMoveAnimation->setDuration(duration);
		mCursorMoveAnimation->setStartValue(QRect(mapToParent(QPoint()).x(), mapToParent(QPoint()).y(), 0, 0));
		mCursorMoveAnimation->setEndValue(QRect(xcoord, ycoord, 0, 0));

		connect (mCursorMoveAnimation, &QPropertyAnimation::finished, mScriptAPI, &ScriptAPI::breakWaiting);

		mCursorMoveAnimation->start();

		mScriptAPI->wait(-1);
	}
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
	if (target) {
		/// Adding 20 to coords needed to navigate in center of targeted item because it haven't size.
		int const xcoord = target->mapTo(parentWidget(), QPoint(0, 0)).x() + xSceneCoord + 20;
		int const ycoord = target->mapTo(parentWidget(), QPoint(0, 0)).y() + ySceneCoord + 20;

		mCursorMoveAnimation->setDuration(duration);
		mCursorMoveAnimation->setStartValue(QRect(pos().x(), pos().y(), 0, 0));
		mCursorMoveAnimation->setEndValue(QRect(xcoord, ycoord, 0, 0));

		connect (mCursorMoveAnimation, &QPropertyAnimation::finished, mScriptAPI, &ScriptAPI::breakWaiting);

		QTimer *timer = new QTimer(this);
		if (mRightButtonPressed) {
			timer->setInterval(100);

			connect(timer, &QTimer::timeout,
					[this, target]() {
						QPoint cursorPos = target->mapFrom(parentWidget(), pos());
						simulateMouse(target, QEvent::MouseMove, cursorPos, Qt::NoButton);
					});
			timer->start();
		}

		mCursorMoveAnimation->start();

		mScriptAPI->wait(-1);
		timer->stop();
	}
}

void VirtualCursor::leftButtonPress(QWidget *target, int delay)
{
	QPoint cursorPos = target->mapFrom(parentWidget(), pos());

	simulateMouse(target, QEvent::MouseButtonPress, cursorPos, Qt::LeftButton);

	if (delay != -1) {
		mScriptAPI->wait(delay);
	}
}

void VirtualCursor::leftButtonRelease(QWidget *target, int delay)
{
	QPoint cursorPos = target->mapFrom(parentWidget(), pos());

	simulateMouse(target, QEvent::MouseButtonRelease, cursorPos, Qt::LeftButton);

	if (delay != -1) {
		mScriptAPI->wait(delay);
	}
}

void VirtualCursor::rightButtonPress(QWidget *target, int delay)
{
	mRightButtonPressed = true;
	QPoint cursorPos = target->mapFrom(parentWidget(), pos());

	simulateMouse(target, QEvent::MouseButtonPress, cursorPos, Qt::RightButton);

	if (delay != -1) {
		mScriptAPI->wait(delay);
	}
}

void VirtualCursor::rightButtonRelease(QWidget *target, int delay)
{
	mRightButtonPressed = false;
	QPoint cursorPos = target->mapFrom(parentWidget(), pos());

	simulateMouse(target, QEvent::MouseButtonRelease, cursorPos, Qt::RightButton);

	if (delay != -1) {
		mScriptAPI->wait(delay);
	}
}

void VirtualCursor::startDrag(QIcon const &icon)
{
	mDragStarted = true;
	mPaletteElementIcon = icon;
	update();
}

void VirtualCursor::endDrag()
{
	 mDragStarted = false;
	 update();
}

void VirtualCursor::moved(QWidget *target)
{
	QPoint cursorPos = target->mapFrom(parentWidget(), pos());

	simulateMouse(target, QEvent::MouseMove, cursorPos, Qt::NoButton);
}

void VirtualCursor::simulateMouse(QObject *reciever, QEvent::Type event, QPointF const &pos
		, Qt::MouseButton button)
{
	QMouseEvent *mouseEvent = new QMouseEvent(event, pos, button, button, Qt::NoModifier);
	QApplication::postEvent(reciever, mouseEvent);
}
