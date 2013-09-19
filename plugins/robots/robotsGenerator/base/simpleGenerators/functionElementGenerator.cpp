#include "functionElementGenerator.h"
#include "../generatorCustomizer.h"

using namespace qReal;
using namespace robots::generators::simple;

FunctionElementGenerator::FunctionElementGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "function.t", QList<Binding *>()
			<< Binding::createConverting("@@BODY@@", "Body", customizer.factory()->functionBlockConverter())
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
