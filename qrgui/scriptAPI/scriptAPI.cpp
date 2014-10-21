#include "scriptAPI.h"

#include <QtCore/QCoreApplication>

#include <qrkernel/exception/exception.h>
#include <qrutils/inFile.h>

#include "mainwindow/mainWindow.h"
#include "view/editorView.h"
#include "controller/commands/createElementCommand.h"

using namespace qReal;
using namespace gui;
using namespace utils;

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
	mScriptEngine.globalObject().setProperty("api", scriptAPI);

	QScriptValue const guiFacade = mScriptEngine.newQObject(mGuiFacade);
	mScriptEngine.globalObject().setProperty("guiFacade", guiFacade);

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

	QString const &fileContent = InFile::readAll(fileName);

	mScriptEngine.setProcessEventsInterval(20);
	mScriptEngine.evaluate(fileContent, fileName);
}

void ScriptAPI::pickComboBoxItem(QComboBox *comboBox, QString const &name, int const duration)
{
	int const comboBoxHeight = comboBox->height()/2;
	int const rowHeight = (comboBox->view()->height() - comboBoxHeight) / comboBox->count();

	int i;
	for (i = 1; i <= comboBox->count(); ++i) {
		if (!comboBox->itemData(i - 1, Qt::DisplayRole).toString().compare(name)) {
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
			,  mVirtualCursor->pos()
			, Qt::LeftButton
			, Qt::LeftButton
			, Qt::NoModifier);
	QEvent *releaseEvent = new QMouseEvent(QMouseEvent::MouseButtonRelease
			, mVirtualCursor->pos()
			, Qt::LeftButton
			, Qt::LeftButton
			, Qt::NoModifier);

	QApplication::postEvent(comboBox->view()->viewport(), pressEvent);
	QApplication::postEvent(comboBox->view()->viewport(), releaseEvent);
	newPos = comboBox->mapTo(parent, mVirtualCursor->pos());
	mVirtualCursor->setParent(parent);
	mVirtualCursor->move(newPos);
	mVirtualCursor->show();
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

QScriptValue ScriptAPI::pluginUi(QString const pluginName)
{
	return mScriptEngine.newQObject(mGuiFacade->pluginGuiFacade(pluginName)
			, QScriptEngine::ScriptOwnership);
}

void ScriptAPI::abortEvaluate()
{
	mScriptEngine.abortEvaluation();
}

GuiFacade *ScriptAPI::guiFacade()
{
	return mGuiFacade;
}

VirtualCursor *ScriptAPI::virtualCursor()
{
	return mVirtualCursor;
}

SceneAPI *ScriptAPI::sceneAPI()
{
	return mSceneAPI;
}

QScriptValue ScriptAPI::ui()
{
	return mScriptEngine.newQObject(mGuiFacade, QScriptEngine::ScriptOwnership);
}

QScriptValue ScriptAPI::hints()
{
	return mScriptEngine.newQObject(mHintAPI, QScriptEngine::ScriptOwnership);
}

QScriptValue ScriptAPI::palette()
{
	return mScriptEngine.newQObject(mPaletteAPI, QScriptEngine::ScriptOwnership);
}

QScriptValue ScriptAPI::scene()
{
	return mScriptEngine.newQObject(mSceneAPI, QScriptEngine::ScriptOwnership);
}

QScriptValue ScriptAPI::cursor()
{
	return mScriptEngine.newQObject(mVirtualCursor, QScriptEngine::ScriptOwnership);
}

QScriptValue ScriptAPI::keyboard()
{
	return mScriptEngine.newQObject(mVirtualKeyboard, QScriptEngine::ScriptOwnership);
}
