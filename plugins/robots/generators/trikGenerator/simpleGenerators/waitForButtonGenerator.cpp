#include "waitForButtonGenerator.h"

#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;

WaitForButtonGenerator::WaitForButtonGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QString const &buttonTemplate
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, buttonTemplate, {}, parent)
{
}
