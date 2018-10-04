/* Copyright 2014-2016 QReal Research Group, Dmitry Chernov, Dmitry Mordvinov, CyberTech Labs Ltd.
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

#include "qrgui/mainWindow/scriptAPI/scriptAPI.h"

#include <QtCore/QPropertyAnimation>
#include <QtGui/QWidgetList>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>

#include <qrkernel/exception/exception.h>
#include <qrkernel/definitions.h>
#include <qrutils/inFile.h>
#include <editor/editorView.h>

#include "qrgui/mainWindow/mainWindow.h"

#include "qrgui/mainWindow/scriptAPI/utils.h"
#include "qrgui/mainWindow/scriptAPI/guiFacade.h"
#include "qrgui/mainWindow/scriptAPI/virtualCursor.h"
#include "qrgui/mainWindow/scriptAPI/virtualKeyboard.h"
#include "qrgui/mainWindow/scriptAPI/hintAPI.h"
#include "qrgui/mainWindow/scriptAPI/sceneAPI.h"
#include "qrgui/mainWindow/scriptAPI/paletteAPI.h"
#include "qrgui/mainWindow/scriptAPI/scriptRegisterMetaTypes.h"

using namespace qReal;
using namespace gui;
using namespace scriptUtils;
using namespace utils;

/// Invokes over time a function passed in parameters with the relative obj.
QScriptValue invokeLater(QScriptContext *context, QScriptEngine *engine) noexcept
{
	Q_UNUSED(engine);
	const QString backtrace = QStringList(context->backtrace().mid(1)).join("\n");
	if (context->argumentCount() < 3) {
		context->throwError(QObject::tr("Function invokeLater(...) must have 3 or more arguments at %1")
				.arg(backtrace));
		return {};
	}

	const int lastButOne = context->argumentCount() - 1;
	if (!(context->argument(0).isValid() && !context->argument(0).toString().isEmpty()
			&& context->argument(0).isQObject() && context->argument(0).toQObject()))
	{
		context->throwError(QObject::tr("Incorrect 'thisObject' name in invokeLater(...) at %1").arg(backtrace));
		return {};
	}

	if (!(context->argument(1).isValid() && context->argument(1).isString()
			&& !context->argument(1).toString().isEmpty()))
	{
		context->throwError(QObject::tr("Incorrect property name in invokeLater(...) at %1").arg(backtrace));
		return {};
	}

	if (!(context->argument(lastButOne).isValid() && context->argument(lastButOne).isNumber()
			&& context->argument(lastButOne).toInt32() > 0))
	{
		context->throwError(QObject::tr("Incorrect waiting time in invokeLater(...) at %1").arg(backtrace));
		return {};
	}

	const QScriptValue thisObject = context->argument(0);
	const QString propertyName = context->argument(1).toString();
	const int msec = context->argument(lastButOne).toInt32();
	QScriptValueList args;
	for (int i = 2; i < lastButOne; ++i) {
		args << context->argument(i);
	}

	lambdaSingleShot(msec, [=]() {
		thisObject.property(propertyName).call(QScriptValue(), args);
	} , thisObject.toQObject());

	return {};
}

ScriptAPI::ScriptAPI()
	: mGuiFacade(nullptr)
	, mVirtualCursor(nullptr)
	, mVirtualKeyboard(nullptr)
	, mSceneAPI(nullptr)
	, mPaletteAPI(nullptr)
	, mHintAPI(nullptr)
	, mUtilsApi(nullptr)
	, mScriptEngine(new QScriptEngine)
{
}

ScriptAPI::~ScriptAPI()
{
}

void ScriptAPI::init(MainWindow &mainWindow)
{
	mGuiFacade.reset(new GuiFacade(mainWindow));
	mVirtualCursor.reset(new VirtualCursor(*this, &mainWindow));
	mVirtualKeyboard.reset(new VirtualKeyboard(*this));
	mSceneAPI.reset(new SceneAPI(*this, mainWindow));
	mPaletteAPI.reset(new PaletteAPI(*this, mainWindow));
	mHintAPI.reset(new HintAPI);
	mUtilsApi.reset(new Utils(*this, mainWindow, *mVirtualCursor.data(), *mVirtualKeyboard.data(), *mHintAPI.data()));
	const QScriptValue scriptAPI = mScriptEngine.newQObject(this);
	// This instance will be available in scripts by writing something like "api.wait(100)"
	mScriptEngine.globalObject().setProperty("api", scriptAPI);

	registerDeclaredTypes(&mScriptEngine);
	registerNewFunction(invokeLater, "invokeLater");
}

void ScriptAPI::evaluate()
{
	const QString fileName(SettingsManager::value("scriptName").toString());
	const QString fileContent = InFile::readAll(fileName);

	mVirtualCursor->show();
	mVirtualCursor->raise();

	mScriptEngine.setProcessEventsInterval(processEventsInterval);
	mScriptEngine.evaluate(fileContent, fileName);
}

void ScriptAPI::evaluateScript(const QString &script, const QString &fileName)
{
	mScriptEngine.setProcessEventsInterval(processEventsInterval);
	mScriptEngine.evaluate(script, fileName);
}

void ScriptAPI::evaluateFileScript(const QString &fileName)
{
	const QString fileContent = InFile::readAll(fileName);
	mScriptEngine.setProcessEventsInterval(processEventsInterval);
	mScriptEngine.evaluate(fileContent, fileName);
}

void ScriptAPI::registerNewFunction(QScriptEngine::FunctionSignature fun, const QString &qScriptName, int length)
{
	Q_UNUSED(length);

	const QScriptValue functionValue = mScriptEngine.newFunction(fun);
	mScriptEngine.globalObject().setProperty(qScriptName, functionValue);
}

QScriptSyntaxCheckResult ScriptAPI::checkSyntax(const QString &script) const
{
	return mScriptEngine.checkSyntax(script);
}

bool ScriptAPI::hasUncaughtException() const
{
	return mScriptEngine.hasUncaughtException();
}

void ScriptAPI::clearExceptions()
{
	return mScriptEngine.clearExceptions();
}

QStringList ScriptAPI::uncaughtExceptionBacktrace() const
{
	return mScriptEngine.uncaughtExceptionBacktrace();
}

QScriptValue ScriptAPI::uncaughtException() const
{
	return mScriptEngine.uncaughtException();
}

QScriptEngine *ScriptAPI::engine()
{
	return &mScriptEngine;
}

void ScriptAPI::pickComboBoxItem(QComboBox *comboBox, const QString &name, int duration) noexcept
{
	const int comboBoxHeight = comboBox->height() / 2;
	const int rowHeight = (comboBox->view()->height() - comboBoxHeight) / comboBox->count();

	int index = 1;
	for (; index <= comboBox->count(); ++index) {
		if (comboBox->itemData(index, Qt::DisplayRole).toString() == name) {
			break;
		}
	}

	const QRect itemRect = comboBox->view()->visualRect(comboBox->view()->indexAt(QPoint(0, rowHeight * index)));
	const QRect target = QRect(itemRect.center(), itemRect.size());

	QWidget *parent = mVirtualCursor->parentWidget();
	QPoint newPos = comboBox->mapFrom(parent, mVirtualCursor->pos());
	mVirtualCursor->setParent(comboBox->view());
	mVirtualCursor->move(newPos);
	mVirtualCursor->raise();
	mVirtualCursor->show();
	QTimer *timer = new QTimer();
	timer->setInterval(100);

	connect(timer, &QTimer::timeout
			, [this, comboBox]() {
				mVirtualCursor->moved(comboBox->view()->viewport());
			} );

	timer->start();
	mVirtualCursor->moveToRect(target, duration);
	timer->stop();

	delete timer;

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


void ScriptAPI::wait(int duration) noexcept
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

void ScriptAPI::switchMouseCursorToWindow(QWidget *parent) noexcept
{
	mVirtualCursor->setParent(parent);
	mVirtualCursor->show();
	mVirtualCursor->leftButtonClick(parent); // places parent in the forefront
}

void ScriptAPI::switchMouseCursorToMainWindow() noexcept
{
	switchMouseCursorToWindow(mGuiFacade->mainWindow());
}

QScriptValue ScriptAPI::pluginUi(const QString &pluginName)
{
	return mScriptEngine.newQObject(mGuiFacade->pluginGuiFacade(pluginName), QScriptEngine::QtOwnership);
}

void ScriptAPI::abortEvaluation()
{
	mScriptEngine.abortEvaluation();
}

GuiFacade &ScriptAPI::guiFacade()
{
	return *mGuiFacade;
}

VirtualCursor &ScriptAPI::virtualCursor()
{
	return *mVirtualCursor;
}

SceneAPI &ScriptAPI::sceneAPI()
{
	return *mSceneAPI;
}

void ScriptAPI::scroll(QAbstractScrollArea *area, QWidget *widget, int duration) noexcept
{
	const int xcoord = area->verticalScrollBar()->parentWidget()->mapToGlobal(area->verticalScrollBar()->pos()).x();
	int ycoord = area->verticalScrollBar()->parentWidget()->mapToGlobal(area->verticalScrollBar()->pos()).y();

	mVirtualCursor->moveToXY(xcoord, ycoord, duration / 2);

	const int diff = area->verticalScrollBar()->height() - area->verticalScrollBar()->pageStep()
			+ widget->pos().y() * area->verticalScrollBar()->maximum() / widget->parentWidget()->height();
	ycoord = ycoord + diff * area->verticalScrollBar()->height() / area->verticalScrollBar()->maximum();

	const QPoint target = mVirtualCursor->parentWidget()->mapFromGlobal(QPoint(xcoord, ycoord));

	QPropertyAnimation *anim = new QPropertyAnimation(area->verticalScrollBar(), "value");
	anim->setDuration(duration / 2);
	anim->setStartValue(0);
	anim->setEndValue(diff);
	connect(anim, &QPropertyAnimation::finished, this, &ScriptAPI::breakWaiting);
	anim->start(QAbstractAnimation::DeleteWhenStopped);

	mVirtualCursor->moveToXY(target.x(), target.y(), duration / 2);
}

QScriptValue ScriptAPI::ui()
{
	return mScriptEngine.newQObject(mGuiFacade.data(), QScriptEngine::QtOwnership);
}

QScriptValue ScriptAPI::hints()
{
	return mScriptEngine.newQObject(mHintAPI.data(), QScriptEngine::QtOwnership);
}

QScriptValue ScriptAPI::palette()
{
	return mScriptEngine.newQObject(mPaletteAPI.data(), QScriptEngine::QtOwnership);
}

QScriptValue ScriptAPI::scene()
{
	return mScriptEngine.newQObject(mSceneAPI.data(), QScriptEngine::QtOwnership);
}

QScriptValue ScriptAPI::cursor()
{
	return mScriptEngine.newQObject(mVirtualCursor.data(), QScriptEngine::QtOwnership);
}

QScriptValue ScriptAPI::keyboard()
{
	return mScriptEngine.newQObject(mVirtualKeyboard.data(), QScriptEngine::QtOwnership);
}

QScriptValue ScriptAPI::utils()
{
	return mScriptEngine.newQObject(mUtilsApi.data(), QScriptEngine::QtOwnership);
}
