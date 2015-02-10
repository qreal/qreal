#include <generatorBase/simpleGenerators/waitForButtonGenerator.h>

#include <generatorBase/generatorCustomizer.h>

using namespace generatorBase::simple;

WaitForButtonGenerator::WaitForButtonGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, const QString &buttonTemplate
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, buttonTemplate, {}, parent)
{
}
