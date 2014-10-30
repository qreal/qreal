#include "waitForColorBlockGenerator.h"
#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;
using namespace qReal;

WaitForColorBlockGenerator::WaitForColorBlockGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "wait/color.t", QList<Binding *>()
			<< Binding::createConverting("@@PORT@@", "Port", customizer.factory()->portNameConverter())
			<< Binding::createConverting("@@COLOR@@", "Color", customizer.factory()->colorConverter())
			, parent)
{
}
