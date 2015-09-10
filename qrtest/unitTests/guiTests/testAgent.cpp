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

#include <QtCore/QDebug>

#include "testAgent.h"

using namespace guiTesting;

TestAgent::TestAgent(QScriptEngine *engine)
	: QScriptEngineAgent(engine)
{
}

void TestAgent::exceptionCatch(qint64 scriptId, const QScriptValue & exception)
{
	Q_UNUSED(scriptId);
	qDebug() << "Agent::exceptionCatch" << exception.toString();
}

void TestAgent::exceptionThrow(qint64 scriptId, const QScriptValue & exception, bool hasHandler)
{
	Q_UNUSED(scriptId);
	qDebug() << "Agent::exceptionThrow" << exception.toString() << hasHandler;
}

void TestAgent::positionChange(qint64 scriptId, int lineNumber, int columnNumber)
{
	Q_UNUSED(scriptId);
	qDebug() << "Agent::positionChange" << lineNumber << " " << columnNumber;
}

void TestAgent::scriptLoad(qint64 id, const QString & program, const QString & fileName, int baseLineNumber)
{
	Q_UNUSED(id);
	Q_UNUSED(program);
	Q_UNUSED(baseLineNumber);
	qDebug() << "Agent::scriptLoad " << fileName;
}

void TestAgent::functionEntry(qint64 scriptId)
{
	Q_UNUSED(scriptId);
	qDebug() << "Agent::functionEntry";
}

void TestAgent::contextPop()
{
	qDebug() << "Agent::contextPop";
}

void TestAgent::contextPush()
{
	qDebug() << "Agent::contextPush";
}
