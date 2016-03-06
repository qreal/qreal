/* Copyright 2015-2016 QReal Research Group, CyberTech Labs Ltd.
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

#include <QtScript/QScriptEngineAgent>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

namespace guiTesting {

/// Helps for debugging and scripting.
class TestAgent : public QScriptEngineAgent
{
public:
	/// @see QScriptEngineAgent.
	explicit TestAgent(QScriptEngine *engine);

	/// Outputs relative info with qInfo().
	/// @see QScriptEngineAgent.
	void exceptionCatch(qint64 scriptId, const QScriptValue & exception) override;

	/// Outputs relative info with qInfo().
	/// @see QScriptEngineAgent.
	void exceptionThrow(qint64 scriptId, const QScriptValue & exception, bool hasHandler) override;

	/// Outputs relative info with qInfo().
	/// @see QScriptEngineAgent.
	void positionChange(qint64 scriptId, int lineNumber, int columnNumber) override;

	/// Outputs relative info with qInfo().
	/// @see QScriptEngineAgent.
	void scriptLoad(qint64 id, const QString & program, const QString & fileName, int baseLineNumber) override;

	/// Outputs relative info with qInfo().
	/// @see QScriptEngineAgent.
	void functionEntry(qint64 scriptId) override;

	/// Outputs relative info with qInfo().
	/// @see QScriptEngineAgent.
	void contextPop() override;

	/// Outputs relative info with qInfo().
	/// @see QScriptEngineAgent.
	void contextPush() override;
};

}
