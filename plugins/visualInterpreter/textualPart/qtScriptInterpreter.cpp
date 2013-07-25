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
