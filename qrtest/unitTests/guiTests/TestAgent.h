/* Copyright 2015 QReal Research Group
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

#include <QScriptEngineAgent>
#include <QScriptValue>
#include <QScriptEngine>
#include <QString>

namespace guiTesting {

class TestAgent : public QScriptEngineAgent
{
public:
	explicit TestAgent(QScriptEngine *engine);

	void exceptionCatch(qint64 scriptId, const QScriptValue & exception) override;
	void exceptionThrow(qint64 scriptId, const QScriptValue & exception, bool hasHandler) override;
	void positionChange(qint64 scriptId, int lineNumber, int columnNumber) override;
	void scriptLoad(qint64 id, const QString & program, const QString & fileName, int baseLineNumber) override;
	void functionEntry(qint64 scriptId) override;
	void contextPop() override;
	void contextPush() override;
};

}
