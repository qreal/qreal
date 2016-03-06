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

#pragma once

#include <QtCore/QObject>

namespace qReal {
namespace gui {

class ScriptAPI;

/// Implements keyboard typing emulation for GUI scripting.
/// @warning You must have parameter with a char (QLatin1Char()) type (without unicode symbols).
/// Otherwise: ASSERT: "false" in file qasciikey.cpp, line 222.
/// @todo In the future this problem should be resolved for testing RuCu, for example.
/// Because of this for guiTest we use adding parameter --no-locale and write only using ascii.
class VirtualKeyboard : public QObject
{
	Q_OBJECT

public:
	explicit VirtualKeyboard(ScriptAPI &mScriptAPI);

	/// Emulates keyboard to type \a message for \a duration time overally.
	Q_INVOKABLE void type(const QString &message, int duration);

	/// Emulates keyboard char click with a modifier.
	/// @param c is ASCII code.
	Q_INVOKABLE void clickKey(QLatin1Char c, Qt::KeyboardModifiers modifier = Qt::NoModifier);

	/// Emulates keyboard "Escape" click.
	Q_INVOKABLE void clickEscape();

private:
	void printValue(const QString &value, int duration);

	ScriptAPI &mScriptAPI;
};

}
}
