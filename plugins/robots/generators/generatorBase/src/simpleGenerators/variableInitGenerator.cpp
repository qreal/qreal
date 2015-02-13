#include "variableInitGenerator.h"
#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;
using namespace qReal;

VariableInitGenerator::VariableInitGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "function.t"
			, { Binding::createStaticConverting("@@BODY@@"
					, repo.property(id, "variable").toString() + "=" + repo.property(id, "value").toString()
					, customizer.factory()->functionBlockConverter(id, "value")) }
			, parent)
{
}
