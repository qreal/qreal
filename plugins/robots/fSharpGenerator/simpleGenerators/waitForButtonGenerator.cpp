#include "waitForButtonGenerator.h"

#include <generatorCustomizer.h>

using namespace qReal::robots::generators::simple;

WaitForButtonGenerator::WaitForButtonGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QString const &buttonTemplate
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, buttonTemplate, {}, parent)
{
}
