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

class MainWidnowScriptAPIInterface
{
public:
	virtual ~MainWidnowScriptAPIInterface() {}

	/// Evaluates script with current ScriptAPI
	/// @param ready script for evaluating
	virtual void evaluateScript(const QString &script) = 0;

	/// Evaluates script located in the file
	/// @param the full path to the file
	virtual void evaluateInFileScript(const QString &fileName) = 0;

	/// Register new function in QtScriptEngine object getting in MainWindow
	/// @param Creates a QScriptValue that wraps a native (C++) function. fun must be a C++ function with signature QScriptEngine::FunctionSignature.
	/// @param length is the number of arguments that fun expects; this becomes the length property of the created QScriptValue.
	virtual void registerNewFunction(QScriptEngine::FunctionSignature fun, int length = 0) = 0;
};
