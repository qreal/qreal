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

#include "scriptAPIWrapper.h"
#include "scriptAPI/scriptAPI.h"

using namespace qReal;
using namespace gui;

ScriptAPIWrapper::ScriptAPIWrapper(ScriptAPI &scriptAPI)
	: mScriptAPI(scriptAPI)
{
}

ScriptAPIWrapper::~ScriptAPIWrapper()
{
}

void ScriptAPIWrapper::evaluateScript(const QString &script, const QString &fileName)
{
	mScriptAPI.evaluateScript(script, fileName);
}

void ScriptAPIWrapper::evaluateFileScript(const QString &fileName)
{
	mScriptAPI.evaluateFileScript(fileName);
}

void ScriptAPIWrapper::abortEvaluation()
{
	mScriptAPI.abortEvaluation();
}

void ScriptAPIWrapper::registerNewFunction(QScriptEngine::FunctionSignature fun, const QString &scriptName, int length)
{
	mScriptAPI.registerNewFunction(fun, scriptName, length);
}

QScriptSyntaxCheckResult ScriptAPIWrapper::checkSyntax(const QString &script) const
{
	return mScriptAPI.checkSyntax(script);
}

bool ScriptAPIWrapper::hasUncaughtException() const
{
	return mScriptAPI.hasUncaughtException();
}

void ScriptAPIWrapper::clearExceptions()
{
	mScriptAPI.clearExceptions();
}

QStringList ScriptAPIWrapper::uncaughtExceptionBacktrace() const
{
	return mScriptAPI.uncaughtExceptionBacktrace();
}

QScriptEngine* ScriptAPIWrapper::engine()
{
	return mScriptAPI.engine();
}
