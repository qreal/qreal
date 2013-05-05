#include "functionElementGenerator.h"

using namespace robots::generator;

FunctionElementGenerator::FunctionElementGenerator(NxtOSEKRobotGenerator *generator
		, qReal::Id const &elementId, bool const &generateToInit)
	: SimpleElementGenerator(generator, elementId), mGenerateToInit(generateToInit)
{
}

void FunctionElementGenerator::variableAnalysis(QString const &code)
{
	QStringList const funcBlocks = code.split(';');

	foreach (QString const &block, funcBlocks) {
			//Only one possible place for first variable appear
		int const firstEqualSignPos = block.indexOf('=');
		if (firstEqualSignPos == -1) {
			continue;
		}

		//must be a normal variable name
		QString leftPart = block.left(firstEqualSignPos);

		leftPart = leftPart.trimmed();
		QString const forbiddenLastSimbols = "+-=*/><";
		if (forbiddenLastSimbols.contains((leftPart.at(leftPart.length() - 1)))) {
			continue;
		}

		bool isVariableExisted = false;
		foreach (SmartLine const &curVariable, mNxtGen->variables()) {
			if (curVariable.text() == leftPart) {
				isVariableExisted = true;
				break;
			}
		}
		if (!isVariableExisted) {
			mNxtGen->variables().append(SmartLine(leftPart, mElementId));
		}
	}
}

QList<SmartLine> FunctionElementGenerator::convertBlockIntoCode()
{
	QList<SmartLine> result;

	qReal::Id const logicElementId = mNxtGen->api()->logicalId(mElementId); //TODO

	QString funcCode = mNxtGen->api()->stringProperty(logicElementId, "Body").toUtf8();
	replaceSensorAndEncoderVariables(funcCode);

	variableAnalysis(funcCode);

	foreach (QString const &str, funcCode.split(';', QString::SkipEmptyParts)) {
		result.append(SmartLine(str.trimmed() + ";", mElementId));
	}

	if (mGenerateToInit) {
		mNxtGen->initCode().append(result);
		result.clear();
	}

	return result;
}
