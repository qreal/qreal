#include "virtualCursor.h"

#include <QtTest/QTest>

#include "scriptAPI.h"
#include "mainwindow/mainWindow.h"
#include "mainwindow/palette/draggableElement.h"

using namespace qReal;
using namespace gui;

VirtualCursor::VirtualCursor(MainWindow *mainWindow, ScriptAPI *scriptAPI)
	: QLabel()
	, mMainWindow(mainWindow)
	, mScriptAPI(scriptAPI)
	, mCursorMoveAnimation (new QPropertyAnimation(this, "geometry"))
	, mRightButtonPressed(false)
	, mIsPaletteElementAttached(false)
{
	setParent(mMainWindow);
	setFixedSize(64, 64);
	show();
	setWindowFlags(Qt::WindowStaysOnTopHint);
	raise();
	update();
}

void VirtualCursor::paintEvent(QPaintEvent *event)
{
	(void)event;
	QPainter mPainter(this);
	QPixmap const virtCursorIcon(":/icons/virtcursor.png");
	mPainter.drawPixmap(0,0, virtCursorIcon	);
	if (mIsPaletteElementAttached) {
		mPainter.drawPixmap(QPoint(32,32), mPaletteElementIcon.pixmap(32, 32));
	}
}

void VirtualCursor::moveTo(QWidget const *target, int duration)
{
	int const xcoord = target->mapTo(parentWidget(), QPoint()).x();
	int const ycoord = target->mapTo(parentWidget(), QPoint()).y();

	mCursorMoveAnimation->setDuration(duration);
	mCursorMoveAnimation->setStartValue(QRect(mapToParent(QPoint()).x(), mapToParent(QPoint()).y(), 0, 0));
	mCursorMoveAnimation->setEndValue(QRect(xcoord, ycoord, 0, 0));

	connect (mCursorMoveAnimation, SIGNAL(finished()), mScriptAPI, SLOT(breakWaiting()));

	mCursorMoveAnimation->start();

	mScriptAPI->wait(-1);
}

void VirtualCursor::moveToRect(QRect target, int duration)
{
	int const xcoord = target.topLeft().x();
	int const ycoord = target.topLeft().y();

	mCursorMoveAnimation->setDuration(duration);
	mCursorMoveAnimation->setStartValue(QRect(pos().x(), pos().y(), 0, 0));
	mCursorMoveAnimation->setEndValue(QRect(xcoord, ycoord, 0, 0));

	connect (mCursorMoveAnimation, SIGNAL(finished()), mScriptAPI, SLOT(breakWaiting()));

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

	if (mRightButtonPressed) {
		connect(mCursorMoveAnimation, SIGNAL(finished()), this, SLOT(edgeFinished()));
		mRightButtonPressed = false;

		QTimer *timer = new QTimer(this);
		timer->setInterval(100);

		connect(timer, &QTimer::timeout,
			[this, target]() {
				QEvent *moveEvent = new QMouseEvent(QMouseEvent::MouseMove, target->mapFromGlobal(pos()), Qt::RightButton , Qt::RightButton, Qt::NoModifier);
				QApplication::postEvent(target, moveEvent);
			});
		timer->start();
	} else {
		connect (mCursorMoveAnimation, SIGNAL(finished()), mScriptAPI, SLOT(breakWaiting()));
	}

	mCursorMoveAnimation->start();

	mScriptAPI->wait(-1);
}

void VirtualCursor::leftButtonPress(QWidget *target, int delay)
{
	QPoint cursorPos = target->mapFrom(parentWidget(), pos());
	QEvent *pressEvent = new QMouseEvent(QMouseEvent::MouseButtonPress, cursorPos, Qt::LeftButton , Qt::LeftButton, Qt::NoModifier);
	QApplication::postEvent(target, pressEvent);

	if (delay != -1) {
		mScriptAPI->wait(delay);
	}
}

void VirtualCursor::leftButtonRelease(QWidget *target, int delay)
{
	QPoint cursorPos = target->mapFrom(parentWidget(), pos());
	QEvent *releaseEvent = new QMouseEvent(QMouseEvent::MouseButtonRelease, cursorPos, Qt::LeftButton , Qt::LeftButton, Qt::NoModifier);
	QApplication::postEvent(target, releaseEvent);

	if (delay != -1) {
		mScriptAPI->wait(delay);
	}
}

void VirtualCursor::rightButtonPress(QWidget *target, int delay)
{
	mRightButtonPressed = true;
	QPoint cursorPos = target->mapFrom(parentWidget(), pos());
	QEvent *pressEvent = new QMouseEvent(QMouseEvent::MouseButtonPress, cursorPos, Qt::RightButton , Qt::RightButton, Qt::NoModifier);
	QApplication::postEvent(target, pressEvent);

	if (delay != -1) {
		mScriptAPI->wait(delay);
	}
}

void VirtualCursor::rightButtonRelease(QWidget  *target, int delay)
{
	mRightButtonPressed = false;
	QPoint cursorPos = target->mapFrom(parentWidget(), pos());
	QEvent *releaseEvent = new QMouseEvent(QMouseEvent::MouseButtonRelease, cursorPos, Qt::RightButton , Qt::RightButton, Qt::NoModifier);
	QApplication::postEvent(target, releaseEvent);

	if (delay != -1) {
		mScriptAPI->wait(delay);
	}
}

void VirtualCursor::drawLink(QString const &fromElementId, QString const &toElementId, int const duration)
{
	NodeElement *fromNode = mMainWindow->getCurrentTab()->editorViewScene()->getNodeById(Id::loadFromString(fromElementId));
	NodeElement *toNode = mMainWindow->getCurrentTab()->editorViewScene()->getNodeById(Id::loadFromString(toElementId));
	sceneMoveTo(mMainWindow->getCurrentTab()->editorViewScene()->views()[0]->viewport(), duration/2, fromNode->pos().x()+ 100, fromNode->pos().y() + 20);

	QTimer *timer = new QTimer(this);
	timer->setInterval(100);

	connect(timer, &QTimer::timeout,
		[this]() {
			QEvent *moveEvent = new QMouseEvent(QMouseEvent::MouseMove, mMainWindow->getCurrentTab()->mapFromGlobal(pos()), Qt::RightButton , Qt::RightButton, Qt::NoModifier);
			QApplication::postEvent(mMainWindow->getCurrentTab()->viewport(), moveEvent);
		});

	QEvent *pressEvent = new QMouseEvent(QMouseEvent::MouseButtonPress, QPoint(fromNode->pos().x()+ 100, fromNode->pos().y() + 20) , Qt::RightButton , Qt::RightButton, Qt::NoModifier);
	QApplication::postEvent(mMainWindow->getCurrentTab()->viewport(), pressEvent);
	mRightButtonPressed = true;

	timer->start();
	sceneMoveTo(mMainWindow->getCurrentTab()->editorViewScene()->views()[0]->viewport(), duration/2, toNode->pos().x() + 100, toNode->pos().y() + 20);
}



void VirtualCursor::virtualDrawing()
{
	QEvent *moveEvent = new QMouseEvent(QMouseEvent::MouseMove, mMainWindow->getCurrentTab()->mapFromGlobal(pos()), Qt::RightButton , Qt::RightButton, Qt::NoModifier);

	QApplication::postEvent(mMainWindow->getCurrentTab()->viewport(), moveEvent);
}

void VirtualCursor::edgeFinished()
{
	QEvent *pressEvent = new QMouseEvent(QMouseEvent::MouseButtonRelease, mMainWindow->getCurrentTab()->mapFromGlobal(pos()) , Qt::RightButton , Qt::RightButton, Qt::NoModifier);
	QApplication::postEvent(mMainWindow->getCurrentTab()->viewport(), pressEvent);
}

QString VirtualCursor::dragPaletteElement(QString const &paletteElementId, int const duration, int const xSceneCoord, int const ySceneCoord)
{
	DraggableElement *paletteElement = mScriptAPI->guiFacade()->draggableElement(paletteElementId);
	QWidget const *target = paletteElement->parentWidget();

	moveTo(target, duration/2);

	attachPaletteElement(paletteElement->icon());

	sceneMoveTo(mMainWindow->getCurrentTab()->editorViewScene()->views()[0]->viewport(), duration/2, xSceneCoord, ySceneCoord);

	detachPaletteElementIcon();


	return createBlockOnScene(paletteElement, xSceneCoord, ySceneCoord);
}

QString VirtualCursor::createBlockOnScene(DraggableElement const *paletteElement, int const xSceneCoord, int const ySceneCoord)
{
	Id elementId(paletteElement->id(), QUuid::createUuid().toString());
	QMimeData *mimeData = paletteElement->mimeData(elementId);
	mMainWindow->getCurrentTab()->editorViewScene()->createElement(
				paletteElement->mimeData(elementId)
				, QPoint(xSceneCoord - 100, ySceneCoord + 20)
				, false
				, nullptr
				, true);

	QByteArray itemData = mimeData->data("application/x-real-uml-data");
	QDataStream inStream(&itemData, QIODevice::ReadOnly);
	QString uuid = "";
	inStream >> uuid;
	Id const sceneId = Id::loadFromString(uuid);
	return sceneId.toString();
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

void VirtualCursor::pickComboBoxItem(QComboBox *comboBox, QString const &name, int const duration)
{
	int const comboBoxHeight = comboBox->height()/2;
	int const rowHeight = (comboBox->view()->height() - comboBoxHeight) / comboBox->count();
	QByteArray const data = name.toLocal8Bit();

	int i;
	for (i = 1; i <= comboBox->count(); ++i) {
		if (!comboBox->itemData(i-1, Qt::DisplayRole).toString().compare(data.data())) {
			break;
		}
	}


	QRect itemRect = comboBox->view()->visualRect(comboBox->view()->indexAt(QPoint(0, rowHeight*i)));
	QRect target = QRect(itemRect.center(), itemRect.size());

	QWidget *parent = parentWidget();
	QPoint newPos = comboBox->mapFrom(parent, pos());
	setParent(comboBox->view());
	this->move(newPos);
	raise();
	show();
	QTimer *timer = new QTimer(this);
	timer->setInterval(100);

	connect(timer, &QTimer::timeout,
		[this, comboBox]() {
			QEvent *moveEvent = new QMouseEvent(QMouseEvent::MouseMove, pos(), Qt::RightButton , Qt::RightButton, Qt::NoModifier);
			QApplication::postEvent(comboBox->view()->viewport(), moveEvent);
		});
	timer->start();
	moveToRect(target, duration);
	timer->stop();

	QEvent *pressEvent = new QMouseEvent(QMouseEvent::MouseButtonPress, QPoint(0, rowHeight*i), Qt::LeftButton , Qt::LeftButton, Qt::NoModifier);
	QEvent *releaseEvent = new QMouseEvent(QMouseEvent::MouseButtonRelease, QPoint(0, rowHeight*i), Qt::LeftButton , Qt::LeftButton, Qt::NoModifier);
	QApplication::postEvent(comboBox->view()->viewport(), pressEvent);
	QApplication::postEvent(comboBox->view()->viewport(), releaseEvent);
	mScriptAPI->wait(400);
	newPos = comboBox->mapTo(parent, pos());
	setParent(parent);
	move(newPos);
	show();
	raise();
}

void VirtualCursor::type(QString const &message, int const duration)
{
	printValue(message, duration);
	QTest::keyClick((QWidget *) nullptr, Qt::Key_Enter, Qt::NoModifier, 200);
	mScriptAPI->wait(300);
}

void VirtualCursor::printValue(QString const &value, int const duration)
{
	int const charDuration = duration / value.length();
	for (QChar ch : value) {
		QTimer *timer = new QTimer(this);
		timer->setInterval(charDuration);
		timer->setSingleShot(true);
		connect(timer, SIGNAL(timeout()), mScriptAPI, SLOT(breakWaiting()));
		timer->start();
		mScriptAPI->wait(-1);
		QTest::keyClicks(nullptr, ch.toLower(), Qt::NoModifier, 0);
	}
}
