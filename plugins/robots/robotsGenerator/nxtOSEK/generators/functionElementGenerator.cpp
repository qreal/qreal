#include "functionElementGenerator.h"

using namespace robots::generator;

FunctionElementGenerator::FunctionElementGenerator(NxtOSEKRobotGenerator *generator
		, qReal::Id const &elementId, bool const &generateToInit)
	: SimpleElementGenerator(generator, elementId), mGenerateToInit(generateToInit)
{
}

QList<SmartLine> FunctionElementGenerator::convertBlockIntoCode()
{
	QList<SmartLine> result;

	qReal::Id const logicElementId = mNxtGen->api()->logicalId(mElementId); //TODO

	QString funcCode = mNxtGen->api()->stringProperty(logicElementId, "Body").toUtf8();
	replaceSensorAndEncoderVariables(funcCode);

	foreach (QString const &str, funcCode.split(';', QString::SkipEmptyParts)) {
		result.append(SmartLine(str.trimmed() + ";", mElementId));
	}

	if (mGenerateToInit) {
		mNxtGen->initCode().append(result);
		result.clear();
	}

	return result;
}
