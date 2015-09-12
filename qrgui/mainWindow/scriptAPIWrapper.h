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

#include <QtScript/QScriptEngine>

namespace qReal {
namespace gui {

class ScriptAPI;

/// @brief ScriptAPIWrapper provides work with ScriptAPI for different classes
class ScriptAPIWrapper
{
public:
	explicit ScriptAPIWrapper(ScriptAPI *scriptAPI);
	~ScriptAPIWrapper();

	/// Evaluates script with current ScriptAPI
	/// @param ready script for evaluating, \a fileName is used for error reporting.
	void evaluateScript(const QString &script, const QString &fileName);

	/// Evaluates script located in the file
	/// @param the full path to the file
	void evaluateFileScript(const QString &fileName);

	/// Abort evaluation of the script
	void abortEvaluation();

	/// Register new function in QtScriptEngine object getting in MainWindow
	/// @param Creates a QScriptValue that wraps a native (C++) function. 
	/// fun must be a C++ function with signature QScriptEngine::FunctionSignature.
	/// @param length is the number of arguments that fun expects;
	/// this becomes the length property of the created QScriptValue.
	void registerNewFunction(QScriptEngine::FunctionSignature fun
									 , const QString &scriptName, int length = 0);

	/// Checks the syntax of the given script.
	/// @return a QScriptSyntaxCheckResult object that contains the result of the check.
	QScriptSyntaxCheckResult checkSyntax(const QString &script);

	/// @return true if the last script evaluation resulted in an uncaught exception; otherwise returns false.
	bool hasUncaughtException();

	/// Clears any uncaught exceptions in corresponding engine.
	void clearExceptions();

	/// @return a human-readable backtrace of the last uncaught exception.
	QStringList uncaughtExceptionBacktrace();

	/// @return QScriptEngine pointer for ScriptAPI engine
	QScriptEngine* getEngine();

private:
	// Doesn't have ownership
	ScriptAPI *mScriptAPI;
};

}
}
