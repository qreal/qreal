#include "functionElementGenerator.h"

#include "generatorBase/generatorCustomizer.h"
#include "generatorBase/parts/functions.h"

using namespace generatorBase::simple;
using namespace qReal;

FunctionElementGenerator::FunctionElementGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "function.t", QList<Binding *>()
			<< Binding::createConverting("@@BODY@@", "Body", customizer.factory()->functionBlockConverter(id, "Body"))
			, parent)
	, mGenerateToInit(false)  // maybe it will be useful one day...
{
}

QString FunctionElementGenerator::generate()
{
	const QString body = BindingGenerator::generate();
	if (mGenerateToInit) {
		mCustomizer.factory()->functions()->registerFunctionInInitialization(body);
		return QString();
	}

	return body;
}
