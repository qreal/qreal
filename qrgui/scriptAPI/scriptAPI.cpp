#include "scriptAPI.h"

#include <QtCore/QPropertyAnimation>
#include <QtCore/qmath.h>
#include <QCoreApplication>

#include <qrkernel/exception/exception.h>

#include "virtualCursor.h"
#include "arrow.h"
#include "guiFacade.h"
#include "mainwindow/palette/draggableElement.h"
#include "controller/commands/createElementCommand.h"
#include "view/editorView.h"

using namespace qReal;
using namespace gui;

ScriptAPI::ScriptAPI(MainWindow *mainWindow)
	: mScriptEngine(new QScriptEngine)
	, mMainWindow (mainWindow)
	, mVirtualCursor (new VirtualCursor(mainWindow))
	, mCursorMoveAnimation (new QPropertyAnimation(mVirtualCursor, "geometry"))
	, mGuiFacade (new GuiFacade(mainWindow))
	, mHintReporter(nullptr)
	, mRightButtonPressed(false)
{
	QScriptValue scriptAPI = mScriptEngine.newQObject(this);
	mScriptEngine.globalObject().setProperty("scriptAPI", scriptAPI);

	QScriptValue robotsGuiFacade = mScriptEngine.newQObject(mGuiFacade->plugin()->guiScriptFacade());
	mScriptEngine.globalObject().setProperty("robotsGuiFacade", robotsGuiFacade);

	QString fileName(":/hintScripts/test.js");

	QFile scriptFile(fileName);
	scriptFile.open(QIODevice::ReadOnly);
	QTextStream stream(&scriptFile);
	mScriptEngine.evaluate(stream.readAll(), fileName);

	scriptFile.close();

	QList<QDialog *> widgetList = mMainWindow->findChildren<QDialog *>();
}

void ScriptAPI::addHint(QString const &message, int const duration)
{
	mHintReporter = new HintReporter(mMainWindow, message, duration);

	QTimer *hintTimer = new QTimer(this);
	connect(hintTimer, SIGNAL(timeout()), this, SLOT(destroyHint()));
	connect(mHintReporter, SIGNAL(mousePressEvent()), this, SLOT(destroyHint()));
	hintTimer->setSingleShot(true);
	hintTimer->start(duration);
}

void ScriptAPI::arrowToWidget(QWidget *target, qreal angle)
{
	int x = target->mapToGlobal(QPoint(0,0)).x();
	int y = target->mapToGlobal(QPoint(0,0)).y();

	int const sourcePointDeviation = 50;
	int const intoTargetDeviation = 50;

	QPoint sourcePoint = QPoint(x - sourcePointDeviation * qSin(angle)
						, y + sourcePointDeviation * qCos(angle));
	QPoint destPoint = QPoint(x ,y );
	Arrow *line = new Arrow(sourcePoint, destPoint, mMainWindow);
	line->show();
}

void ScriptAPI::moveVirtualCursor(QString const &type, QString const &target, int duration, int xSceneCoord, int ySceneCoord)
{
	QWidget *targetWidget;

	int xcoord = 0;
	int ycoord = 0;

	if (!type.compare("Action")) {
		targetWidget = mGuiFacade->widget("Action", target);
		xcoord = targetWidget->mapToGlobal(QPoint(0, 0)).x() + targetWidget->width()/2;
		ycoord = targetWidget->mapToGlobal(QPoint(0, 0)).y();
	} else if (!type.compare("DraggableElement")) {
		targetWidget = mGuiFacade->draggableElement(target)->parentWidget();
		xcoord = targetWidget->mapToGlobal(QPoint(0, 0)).x() + targetWidget->width()/2;
		ycoord = targetWidget->mapToGlobal(QPoint(0, 0)).y() + targetWidget->height()/2;
	} else if (!type.compare("Scene")) {
		EditorView *tab = mMainWindow->getCurrentTab();
		xcoord = tab->mapToGlobal(QPoint(0, 0)).x() + xSceneCoord;
		ycoord = tab->mapToGlobal(QPoint(0, 0)).y() + ySceneCoord;
	} else if (!type.compare("2dModel")) {
		targetWidget = mGuiFacade->widget(type, target);
		xcoord = targetWidget->mapToGlobal(QPoint(0, 0)).x() + targetWidget->width()/2;
		ycoord = targetWidget->mapToGlobal(QPoint(0, 0)).y() + targetWidget->height()/2;
	}


	mCursorMoveAnimation->setDuration(duration);
	mCursorMoveAnimation->setStartValue(QRect(mVirtualCursor->x(), mVirtualCursor->y(), 0, 0));
	mCursorMoveAnimation->setEndValue(QRect(xcoord, ycoord, 0, 0));
	mCursorMoveAnimation->start();

	connect (mCursorMoveAnimation, SIGNAL(finished()), &mVirtualMouseEventLoop, SLOT(quit()));
	if (mRightButtonPressed) {
		connect(mCursorMoveAnimation, SIGNAL(finished()), this, SLOT(edgeFinished()));
		mRightButtonPressed = false;
	} else {
		connect (mCursorMoveAnimation, SIGNAL(finished()), &mVirtualMouseEventLoop, SLOT(quit()));
	}
	mVirtualMouseEventLoop.processEvents(QEventLoop::WaitForMoreEvents);
	mVirtualMouseEventLoop.exec();
}

void ScriptAPI::virtualCursorLeftClick(QString const &type, QString const &target)
{
	QWidget *targetWidget;

	if (!type.compare("Action")) {
		targetWidget = mGuiFacade->widget("Action", target);
	} else if (!type.compare("DraggableElement")) {
		targetWidget = mGuiFacade->draggableElement(target)->parentWidget();
	} else if (!type.compare("2dModel")) {
		targetWidget = mGuiFacade->widget(type, target);
	}

	QEvent *pressEvent = new QMouseEvent(QMouseEvent::MouseButtonPress, QPoint(0, 0), Qt::LeftButton , Qt::LeftButton, Qt::NoModifier);
	QEvent *releaseEvent = new QMouseEvent(QMouseEvent::MouseButtonRelease, QPoint(0, 0), Qt::LeftButton , Qt::LeftButton, Qt::NoModifier);

	QApplication::postEvent(targetWidget, pressEvent);
	QApplication::postEvent(targetWidget, releaseEvent);
}

QString ScriptAPI::dragPaletteElement(QString const &paletteElementId, int const duration, int const xSceneCoord, int const ySceneCoord)
{
	moveVirtualCursor("DraggableElement", paletteElementId, duration/2);

	DraggableElement const *paletteElement = mGuiFacade->draggableElement(paletteElementId);
	mVirtualCursor->attachPaletteElement(paletteElement->icon());

	moveVirtualCursor("Scene", "", duration/2, xSceneCoord, ySceneCoord);

	mVirtualCursor->detachPaletteElementIcon();

	return createBlockOnScene(paletteElement, xSceneCoord, ySceneCoord);
}

QString ScriptAPI::createBlockOnScene(DraggableElement const *paletteElement, int const xSceneCoord, int const ySceneCoord)
{
	Id elementId(paletteElement->id(), QUuid::createUuid().toString());
	QMimeData *mimeData = paletteElement->mimeData(elementId);
	mMainWindow->getCurrentTab()->editorViewScene()->createElement(
				paletteElement->mimeData(elementId)
				, QPoint(xSceneCoord, ySceneCoord)
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

void ScriptAPI::drawLink(QString const &fromElementId, QString const &toElementId, int const duration)
{
	NodeElement *fromNode = mMainWindow->getCurrentTab()->editorViewScene()->getNodeById(Id::loadFromString(fromElementId));
	NodeElement *toNode = mMainWindow->getCurrentTab()->editorViewScene()->getNodeById(Id::loadFromString(toElementId));

	moveVirtualCursor("Scene", "", duration/2, fromNode->pos().x()+ 100, fromNode->pos().y() + 20);

	QTimer *timer = new QTimer(this);
	timer->setInterval(100);
	connect(timer, SIGNAL(timeout()), this, SLOT(virtualDrawing()));

	QEvent *pressEvent = new QMouseEvent(QMouseEvent::MouseButtonPress, QPoint(fromNode->pos().x()+ 100, fromNode->pos().y() + 20) , Qt::RightButton , Qt::RightButton, Qt::NoModifier);
	QApplication::postEvent(mMainWindow->getCurrentTab()->viewport(), pressEvent);
	mRightButtonPressed = true;
	timer->start();
	moveVirtualCursor("Scene", "", duration/2, toNode->pos().x() + 100, toNode->pos().y() + 20);
}

void ScriptAPI::destroyHint()
{
	delete mHintReporter;
}

void ScriptAPI::virtualDrawing()
{
	int xcoord =  mMainWindow->getCurrentTab()->mapFromGlobal(mVirtualCursor->mapToGlobal(QPoint(0, 0))).x();
	int ycoord =  mMainWindow->getCurrentTab()->mapFromGlobal(mVirtualCursor->mapToGlobal(QPoint(0, 0))).y();

	QEvent *moveEvent = new QMouseEvent(QMouseEvent::MouseMove, QPoint(xcoord, ycoord), Qt::RightButton , Qt::RightButton, Qt::NoModifier);

	QApplication::postEvent(mMainWindow->getCurrentTab()->viewport(), moveEvent);
}

void ScriptAPI::edgeFinished()
{
	QEvent *pressEvent = new QMouseEvent(QMouseEvent::MouseButtonRelease, mMainWindow->getCurrentTab()->mapFromGlobal(mVirtualCursor->pos()) , Qt::RightButton , Qt::RightButton, Qt::NoModifier);
	QApplication::postEvent(mMainWindow->getCurrentTab()->viewport(), pressEvent);

	mVirtualMouseEventLoop.quit();
}

void ScriptAPI::open2dModel(QWidget *pluginWidget)
{
	moveVirtualCursor("2dModel", "open", 1000);
	virtualCursorLeftClick("2dModel", "open");
	mVirtualCursor->setParent(pluginWidget);
}

void ScriptAPI::close2dModel()
{

}
