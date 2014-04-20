#include "trikEnginesStopGenerator.h"

#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;
using namespace generatorBase::simple;

TrikEnginesStopGenerator::TrikEnginesStopGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "engines/power/stop.t", QList<Binding *>()
			<< Binding::createMultiTarget("@@PORT@@", "Ports", customizer.factory()->enginesConverter())
			, parent)
{
}
