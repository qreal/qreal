#include <generatorBase/simpleGenerators/waitForButtonGenerator.h>

#include <generatorBase/generatorCustomizer.h>

using namespace generatorBase::simple;

WaitForButtonGenerator::WaitForButtonGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "wait/button.t"
			, {Binding::createConverting("@@BUTTON@@", "Button", customizer.factory()->portNameConverter())}
			, parent)
{
}
