#include "waitForColorBlockGenerator.h"
#include "generatorBase/generatorCustomizer.h"

using namespace qReal::robots::generators::simple;

WaitForColorBlockGenerator::WaitForColorBlockGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "wait/color.t", QList<Binding *>()
			<< Binding::createConverting("@@PORT@@", "Port", customizer.factory()->inputPortConverter())
			<< Binding::createConverting("@@COLOR@@", "Color", customizer.factory()->colorConverter())
			, parent)
{
}
