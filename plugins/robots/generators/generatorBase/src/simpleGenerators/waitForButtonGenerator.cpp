#include <generatorBase/simpleGenerators/waitForButtonGenerator.h>

#include <generatorBase/generatorCustomizer.h>

using namespace generatorBase::simple;

WaitForButtonGenerator::WaitForButtonGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "wait/button.t"
			, {Binding::createConverting("@@BUTTON@@", "Button", customizer.factory()->portNameConverter())}
			, parent)
{
}
