#include "functionElementGenerator.h"

using namespace robots::trikGenerator;

FunctionElementGenerator::FunctionElementGenerator(TrikRobotGenerator *generator
		, qReal::Id const &elementId, bool const &generateToInit)
	: SimpleElementGenerator(generator, elementId), mGenerateToInit(generateToInit)
{
}

QList<SmartLine> FunctionElementGenerator::convertBlockIntoCode()
{
	QList<SmartLine> result;

	qReal::Id const logicElementId = mTrikGenerator->api()->logicalId(mElementId); //TODO

	QString funcCode = mTrikGenerator->api()->stringProperty(logicElementId, "Body").toUtf8();
	funcCode = replaceSensorAndEncoderVariables(funcCode);
	funcCode = replaceFunctionInvocations(funcCode);

	foreach (QString const &str, funcCode.split(';', QString::SkipEmptyParts)) {
		result.append(SmartLine(str.trimmed() + ";", mElementId));
	}

//	if (mGenerateToInit) {
//		mTrikGenerator->initCode().append(result);
//		result.clear();
//	}

	return result;
}
