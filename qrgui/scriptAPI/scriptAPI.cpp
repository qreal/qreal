#include "scriptAPI.h"

#include <QtTest/QTest>
#include <QCoreApplication>

#include <qrkernel/exception/exception.h>

#include "mainwindow/mainWindow.h"
#include "view/editorView.h"
#include "controller/commands/createElementCommand.h"

using namespace qReal;
using namespace gui;

ScriptAPI::ScriptAPI()
	: mGuiFacade (nullptr)
	, mVirtualCursor (nullptr)
	, mVirtualKeyboard (nullptr)
	, mSceneAPI(nullptr)
	, mPaletteAPI(nullptr)
	, mHintAPI(nullptr)
	, mMainWindow(nullptr)
	, mScriptEngine(new QScriptEngine)
{
}

ScriptAPI::~ScriptAPI()
{
	delete mHintAPI;
	delete mPaletteAPI;
	delete mSceneAPI;
	delete mVirtualKeyboard;
	delete mVirtualCursor;
	delete mGuiFacade;
}

void ScriptAPI::init(MainWindow *mainWindow)
{
	mMainWindow = mainWindow;
	mGuiFacade = new GuiFacade(mainWindow);
	mVirtualCursor = new VirtualCursor(this, mMainWindow);
	mVirtualKeyboard = new VirtualKeyboard(this);
	mSceneAPI = new SceneAPI(this, mainWindow);
	mPaletteAPI = new PaletteAPI(this, mainWindow);
	mHintAPI = new HintAPI(this);

	QScriptValue const scriptAPI = mScriptEngine.newQObject(this);
	mScriptEngine.globalObject().setProperty("API", scriptAPI);

	QScriptValue const guiFacade = mScriptEngine.newQObject(mGuiFacade);
	mScriptEngine.globalObject().setProperty("guiFacade", guiFacade);

	QScriptValue const robotsGuiFacade = mScriptEngine.newQObject(mGuiFacade->pluginGuiFacade("qRealRobots.RobotsPlugin"));
	mScriptEngine.globalObject().setProperty("pluginGuiFacade", robotsGuiFacade);

	QScriptValue const virtualCursor = mScriptEngine.newQObject(mVirtualCursor);
	mScriptEngine.globalObject().setProperty("cursor", virtualCursor);

	QScriptValue const virtualKeyboard = mScriptEngine.newQObject(mVirtualKeyboard);
	mScriptEngine.globalObject().setProperty("keyboard", virtualKeyboard);

	QScriptValue const paletteAPI = mScriptEngine.newQObject(mPaletteAPI);
	mScriptEngine.globalObject().setProperty("paletteAPI", paletteAPI);

	QScriptValue const hintAPI = mScriptEngine.newQObject(mHintAPI);
	mScriptEngine.globalObject().setProperty("hintAPI", hintAPI);

	QScriptValue const sceneAPI = mScriptEngine.newQObject(mSceneAPI);
	mScriptEngine.globalObject().setProperty("sceneAPI", sceneAPI);

	QString const fileName(SettingsManager::value("scriptName").toString());
	QFile scriptFile(fileName);
	scriptFile.open(QIODevice::ReadOnly);
	QTextStream stream(&scriptFile);

	mScriptEngine.setProcessEventsInterval(20);
	mScriptEngine.evaluate(stream.readAll(), fileName);

	scriptFile.close();
}

void ScriptAPI::pickComboBoxItem(QComboBox *comboBox, QString const &name, int const duration)
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

	QWidget *parent = mVirtualCursor->parentWidget();
	QPoint newPos = comboBox->mapFrom(parent, mVirtualCursor->pos());
	mVirtualCursor->setParent(comboBox->view());
	mVirtualCursor->move(newPos);
	mVirtualCursor->raise();
	mVirtualCursor->show();
	QTimer *timer = new QTimer(this);
	timer->setInterval(100);

	connect(timer, &QTimer::timeout,
		[this, comboBox]() {
			mVirtualCursor->moved(comboBox->view()->viewport());
		});
	timer->start();
	mVirtualCursor->moveToRect(target, duration);
	timer->stop();

	QEvent *pressEvent = new QMouseEvent(QMouseEvent::MouseButtonPress
										 , QPoint(0, rowHeight*i)
										 , Qt::LeftButton
										 , Qt::LeftButton
										 , Qt::NoModifier);
	QEvent *releaseEvent = new QMouseEvent(QMouseEvent::MouseButtonRelease
										   , QPoint(0, rowHeight*i)
										   , Qt::LeftButton
										   , Qt::LeftButton
										   , Qt::NoModifier);
	QApplication::postEvent(comboBox->view()->viewport(), pressEvent);
	QApplication::postEvent(comboBox->view()->viewport(), releaseEvent);
	wait(200);
	newPos = comboBox->mapTo(parent, mVirtualCursor->pos());
	mVirtualCursor->setParent(parent);
	mVirtualCursor->move(newPos);
	mVirtualCursor->show();
	mVirtualCursor->raise();
}


void ScriptAPI::wait(int duration)
{
	if (duration != -1) {
		QTimer::singleShot(duration, &mEventLoop, SLOT(quit()));
	}

	mEventLoop.exec();
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

void ScriptAPI::abortEvaluate()
{
	mScriptEngine.abortEvaluation("Aborted");
}

GuiFacade *ScriptAPI::guiFacade()
{
	return mGuiFacade;
}

VirtualCursor *ScriptAPI::virtualCursor()
{
	return mVirtualCursor;
}

VirtualKeyboard *ScriptAPI::virtualKeyboard()
{
	return mVirtualKeyboard;
}

HintAPI *ScriptAPI::hintAPI()
{
	return mHintAPI;
}

SceneAPI *ScriptAPI::sceneAPI()
{
	return mSceneAPI;
}

PaletteAPI *ScriptAPI::paletteAPI()
{
	return mPaletteAPI;
}
