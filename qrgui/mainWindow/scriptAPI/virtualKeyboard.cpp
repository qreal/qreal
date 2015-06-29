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

#include "virtualKeyboard.h"

#include <QtTest/QTest>
#include <QtCore/QTimer>

#include "scriptAPI.h"

using namespace qReal;
using namespace gui;

VirtualKeyboard::VirtualKeyboard(ScriptAPI &scriptAPI)
	: mScriptAPI(scriptAPI)
{
}

void VirtualKeyboard::type(const QString &message, int duration)
{
	if (QApplication::focusWidget()) {
		printValue(message, duration);
		QTest::keyClick(QApplication::focusWidget(), Qt::Key_Enter, Qt::NoModifier, 0);
		mScriptAPI.wait(300);
	}
}

void VirtualKeyboard::printValue(const QString &value, int duration)
{
	const int charDuration = duration / value.length();
	for (QChar ch : value) {
		QTimer *timer = new QTimer(this);
		timer->setInterval(charDuration);
		timer->setSingleShot(true);
		connect(timer, &QTimer::timeout, &mScriptAPI, &ScriptAPI::breakWaiting);
		timer->start();
		mScriptAPI.wait(-1);
		QTest::keyClicks(nullptr, ch.toLower(), Qt::NoModifier, 0);
	}
}
