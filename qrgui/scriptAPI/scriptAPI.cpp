#include "scriptAPI.h"

#include <QtTest/QTest>
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
	, mVirtualCursor (new VirtualCursor(mainWindow, this))
	, mGuiFacade (new GuiFacade(mainWindow))
	, mHintReporter(nullptr)
{
	QScriptValue scriptAPI = mScriptEngine.newQObject(this);
	mScriptEngine.globalObject().setProperty("scriptAPI", scriptAPI);

	QScriptValue robotsGuiFacade = mScriptEngine.newQObject(mGuiFacade->plugin()->guiScriptFacade());
	mScriptEngine.globalObject().setProperty("robotsGuiFacade", robotsGuiFacade);

	QScriptValue guiFacade = mScriptEngine.newQObject(mGuiFacade);
	mScriptEngine.globalObject().setProperty("guiFacade", guiFacade);

	QScriptValue virtualCursor = mScriptEngine.newQObject(mVirtualCursor);
	mScriptEngine.globalObject().setProperty("virtualCursor", virtualCursor);

	QString fileName(":/hintScripts/obstacle.js");

	QFile scriptFile(fileName);
	scriptFile.open(QIODevice::ReadOnly);
	QTextStream stream(&scriptFile);

	mScriptEngine.evaluate(stream.readAll(), fileName);

	scriptFile.close();
}

void ScriptAPI::addHint(QString const &message, int const duration)
{
	QByteArray data = message.toLocal8Bit();
	QString modifiedMessage = data.data();
	mHintReporter = new HintReporter(mMainWindow, modifiedMessage, duration);

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

	QPoint sourcePoint = QPoint(x - sourcePointDeviation * qSin(angle)
						, y + sourcePointDeviation * qCos(angle));
	QPoint destPoint = QPoint(x ,y );
	Arrow *line = new Arrow(sourcePoint, destPoint, mMainWindow);
	line->show();
}

void ScriptAPI::destroyHint()
{
	delete mHintReporter;
}

void ScriptAPI::wait(int duration)
{
	if (duration != -1) {
		QTimer *timer = new QTimer(this);
		timer->setSingleShot(true);
		timer->setInterval(duration);
		connect (timer, SIGNAL(timeout()), &mEventLoop, SLOT(quit()));
		timer->start();
		mEventLoop.processEvents(QEventLoop::WaitForMoreEvents);
		mEventLoop.exec();
	} else {
		mEventLoop.processEvents(QEventLoop::WaitForMoreEvents);
		mEventLoop.exec();
	}
}

GuiFacade *ScriptAPI::guiFacade()
{
	return mGuiFacade;
}

void ScriptAPI::breakWaiting()
{
	mEventLoop.quit();
}

void ScriptAPI::changeWindow(QWidget *parent)
{
	mVirtualCursor->setParent(parent);
	mVirtualCursor->show();
	mVirtualCursor->leftButtonPress(parent);
	mVirtualCursor->leftButtonRelease(parent);
}


QString ScriptAPI::initialNode()
{
	QList<QGraphicsItem *> items = mMainWindow->getCurrentTab()->editorViewScene()->items();
	foreach (QGraphicsItem *item, items) {
		NodeElement *node = dynamic_cast<NodeElement*>(item);
		if (node ) {
			if (!node->id().diagram().compare("RobotsDiagram") && !node->id().element().compare("InitialNode")) {
				return node->id().toString();
			}
		}
	}
	return "";
}
