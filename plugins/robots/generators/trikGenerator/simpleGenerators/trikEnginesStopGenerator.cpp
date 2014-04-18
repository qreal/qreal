#include "trikEnginesStopGenerator.h"

#include <generatorBase/generatorCustomizer.h>

using namespace qReal::robots::generators::simple;

TrikEnginesStopGenerator::TrikEnginesStopGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "engines/power/stop.t", QList<Binding *>()
			<< Binding::createMultiTarget("@@PORT@@", "Ports", customizer.factory()->enginesConverter())
			, parent)
{
}
