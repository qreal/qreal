#include "enginesStopGenerator.h"
#include "../generatorCustomizer.h"

using namespace qReal::robots::generators::simple;

EnginesStopGenerator::EnginesStopGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "engines/enginesStop.t", QList<Binding *>()
			<< Binding::createMultiTarget("@@PORT@@", "Ports", customizer.factory()->enginesConverter())
			, parent)
{
}
