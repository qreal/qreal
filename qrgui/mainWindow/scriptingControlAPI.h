/* Copyright 2015-2015 QReal Research Group, CyberTech Labs Ltd.
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

#include <QtScript/QScriptEngine>

namespace qReal {
namespace gui {

class ScriptAPI;

/// @brief This is a brief scripting control API for other QReal components.
class ScriptingControlAPI
{
public:
	explicit ScriptingControlAPI(ScriptAPI &scriptAPI);
	~ScriptingControlAPI();

	/// Evaluates script with current ScriptAPI.
	/// @param ready script for evaluating, \a fileName is used for error reporting.
	void evaluateScript(const QString &script, const QString &fileName);

	/// Evaluates script located in the file.
	/// @param the full path to the file.
	void evaluateFileScript(const QString &fileName);

	/// Aborts evaluation of the script.
	void abortEvaluation();

	/// Registers new function in QtScriptEngine object getting in MainWindow.
	/// @param Creates a QScriptValue that wraps a native (C++) function. 
	/// \a fun must be a C++ function with signature QScriptEngine::FunctionSignature.
	/// @param length is the number of arguments that fun expects;
	/// This becomes the length property of the created QScriptValue.
	void registerNewFunction(QScriptEngine::FunctionSignature fun
			, const QString &scriptName, int length = 0);

	/// Checks the syntax of the given script.
	/// @return a QScriptSyntaxCheckResult object that contains the result of the check.
	QScriptSyntaxCheckResult checkSyntax(const QString &script) const;

	/// @return true if the last script evaluation resulted in an uncaught exception; otherwise returns false.
	bool hasUncaughtException() const;

	/// Clears any uncaught exceptions in corresponding engine.
	void clearExceptions();

	/// @return a human-readable backtrace of the last uncaught exception.
	QStringList uncaughtExceptionBacktrace() const;

	/// @return the current uncaught exception, or an invalid QScriptValue if there is no uncaught exception.
	/// The exception value is typically an Error object; in that case,
	/// you can call toString() on the return value to obtain an error message.
	QScriptValue uncaughtException() const;

	/// @return QScriptEngine pointer for ScriptAPI engine.
	QScriptEngine *engine();

private:
	ScriptAPI &mScriptAPI;
};

}
}
