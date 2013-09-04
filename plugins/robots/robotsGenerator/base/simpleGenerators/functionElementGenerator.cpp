#include "functionElementGenerator.h"
#include "../generatorCustomizer.h"

using namespace qReal;
using namespace robots::generators::simple;

FunctionElementGenerator::FunctionElementGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, bool generateToInit
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "function.t", QList<Binding *>()
			<< Binding::createConverting("@@BODY@@", "Body", customizer.factory()->functionBlockConverter())
			, parent)
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
