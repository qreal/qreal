#include <generatorBase/simpleGenerators/waitForButtonGenerator.h>

#include <generatorBase/generatorCustomizer.h>

using namespace generatorBase::simple;

WaitForButtonGenerator::WaitForButtonGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QString const &buttonTemplate
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, buttonTemplate, {}, parent)
{
}
