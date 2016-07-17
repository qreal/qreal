/* Copyright 2014-2015 QReal Research Group, Dmitry Chernov, Dmitry Mordvinov
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

#include "scriptAPI.h"

#include <QtCore/QPropertyAnimation>
#include <QtGui/QWidgetList>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>

#include <qrkernel/exception/exception.h>
#include <qrutils/inFile.h>
#include <editor/editorView.h>

#include "qrgui/mainWindow/mainWindow.h"

#include "guiFacade.h"
#include "virtualCursor.h"
#include "virtualKeyboard.h"
#include "hintAPI.h"
#include "sceneAPI.h"
#include "paletteAPI.h"

using namespace qReal;
using namespace gui;
using namespace utils;

ScriptAPI::ScriptAPI()
	: mGuiFacade(nullptr)
	, mVirtualCursor(nullptr)
	, mVirtualKeyboard(nullptr)
	, mSceneAPI(nullptr)
	, mPaletteAPI(nullptr)
	, mHintAPI(nullptr)
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

	const QScriptValue scriptAPI = mScriptEngine.newQObject(this);
	// This instance will be available in scripts by writing something like "api.wait(100)"
	mScriptEngine.globalObject().setProperty("api", scriptAPI);
}

void ScriptAPI::evaluate()
{
	const QString fileName(SettingsManager::value("scriptName").toString());
	const QString fileContent = InFile::readAll(fileName);

	mVirtualCursor->show();
	mVirtualCursor->raise();

	mScriptEngine.setProcessEventsInterval(20);
	mScriptEngine.evaluate(fileContent, fileName);

	abortEvaluation();
}

void ScriptAPI::pickComboBoxItem(QComboBox *comboBox, const QString &name, int duration)
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
			});

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

void ScriptAPI::scroll(QAbstractScrollArea *area, QWidget *widget, int duration)
{
	const int xcoord = area->verticalScrollBar()->parentWidget()->mapToGlobal(area->verticalScrollBar()->pos()).x();
	int ycoord = area->verticalScrollBar()->parentWidget()->mapToGlobal(area->verticalScrollBar()->pos()).y();

	mVirtualCursor->moveToPoint(xcoord, ycoord, duration / 2);

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

	mVirtualCursor->moveToPoint(target.x(), target.y(), duration / 2);
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
