/* Copyright 2007-2015 QReal Research Group
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

#include "qtScriptInterpreter.h"
#include "textCodeGenerator.h"

using namespace qReal;

QtScriptInterpreter::QtScriptInterpreter(QObject *parent) : TextCodeInterpreter(parent)
{
}

bool QtScriptInterpreter::interpret(QString const &code, CodeType const codeType)
{
	QString const output = mEngine.evaluate(code).toString();

	if (codeType != initialization) {
		processOutput(output);
	}

	if (codeType == applicationCondition) {
		return mApplicationConditionResult && !mErrorOccured;
	} else {
		return !mErrorOccured;
	}
}

void QtScriptInterpreter::processOutput(QString const &outputString)
{
	if (outputString.isEmpty() || outputString == "undefined") {
		return;
	}

	mErrorOccured = false;
	if (outputString == "true") {
		mApplicationConditionResult = true;
	} else if (outputString == "false") {
		mApplicationConditionResult = false;
	} else {
		QHash<QPair<QString, QString>, QString> output = parseOutput(outputString);

		if (!output.isEmpty()) {
			emit readyReadStdOutput(output, TextCodeInterpreter::qtScript);
		} else {
			mErrorOccured = true;
			emit readyReadErrOutput(outputString);
		}
	}
}
