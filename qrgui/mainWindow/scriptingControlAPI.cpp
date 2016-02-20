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

#include "scriptingControlAPI.h"
#include "scriptAPI/scriptAPI.h"

using namespace qReal;
using namespace gui;

ScriptingControlAPI::ScriptingControlAPI(ScriptAPI &scriptAPI)
	: mScriptAPI(scriptAPI)
{
}

ScriptingControlAPI::~ScriptingControlAPI()
{
}

void ScriptingControlAPI::evaluateScript(const QString &script, const QString &fileName)
{
	mScriptAPI.evaluateScript(script, fileName);
}

void ScriptingControlAPI::evaluateFileScript(const QString &fileName)
{
	mScriptAPI.evaluateFileScript(fileName);
}

void ScriptingControlAPI::abortEvaluation()
{
	mScriptAPI.abortEvaluation();
}

void ScriptingControlAPI::registerNewFunction(QScriptEngine::FunctionSignature fun, const QString &scriptName, int length)
{
	mScriptAPI.registerNewFunction(fun, scriptName, length);
}

QScriptSyntaxCheckResult ScriptingControlAPI::checkSyntax(const QString &script) const
{
	return mScriptAPI.checkSyntax(script);
}

bool ScriptingControlAPI::hasUncaughtException() const
{
	return mScriptAPI.hasUncaughtException();
}

void ScriptingControlAPI::clearExceptions()
{
	mScriptAPI.clearExceptions();
}

QStringList ScriptingControlAPI::uncaughtExceptionBacktrace() const
{
	return mScriptAPI.uncaughtExceptionBacktrace();
}

QScriptValue ScriptingControlAPI::uncaughtException() const
{
	return mScriptAPI.uncaughtException();
}

QScriptEngine *ScriptingControlAPI::engine()
{
	return mScriptAPI.engine();
}
