#include "functionElementGenerator.h"

using namespace qReal;
using namespace robots::generators::simple;

FunctionElementGenerator::FunctionElementGenerator(LogicalModelAssistInterface const &model
		, GeneratorCustomizer &customizer
		, Id const &id
		, bool generateToInit
		, QObject *parent)
	: BindingGenerator(model, customizer, id, "function.t", QList<Binding *>(), parent)
	, mGenerateToInit(generateToInit)
{
}

//QList<SmartLine_old> FunctionElementGenerator::convertBlockIntoCode()
//{
//	QList<SmartLine_old> result;

//	qReal::Id const logicElementId = mNxtGen->api()->logicalId(mElementId); //TODO

//	QString funcCode = mNxtGen->api()->stringProperty(logicElementId, "Body").toUtf8();
//	funcCode = replaceSensorAndEncoderVariables(funcCode);
//	funcCode = replaceFunctionInvocations(funcCode);

//	foreach (QString const &str, funcCode.split(';', QString::SkipEmptyParts)) {
//		result.append(SmartLine_old(str.trimmed() + ";", mElementId));
//	}

//	if (mGenerateToInit) {
//		mNxtGen->initCode().append(result);
//		result.clear();
//	}

//	return result;
//}
