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
