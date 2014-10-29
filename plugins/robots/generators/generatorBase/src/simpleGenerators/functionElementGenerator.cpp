#include "functionElementGenerator.h"

#include "generatorBase/generatorCustomizer.h"
#include "generatorBase/parts/functions.h"

using namespace generatorBase::simple;
using namespace qReal;

FunctionElementGenerator::FunctionElementGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "function.t", QList<Binding *>()
			<< Binding::createConverting("@@BODY@@", "Body", customizer.factory()->functionBlockConverter(id, "Body"))
			, parent)
	, mGenerateToInit(mRepo.property(id, "Init").toBool())
{
}

QString FunctionElementGenerator::generate()
{
	QString const body = BindingGenerator::generate();
	if (mGenerateToInit) {
		mCustomizer.factory()->functions()->registerFunctionInInitialization(body);
		return QString();
	}

	return body;
}
