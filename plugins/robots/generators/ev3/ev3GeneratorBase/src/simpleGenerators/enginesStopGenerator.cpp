#include "enginesStopGenerator.h"
#include "src/converters/portNameConverter.h"

#include <generatorBase/generatorCustomizer.h>

using namespace ev3::simple;
using namespace generatorBase::simple;

EnginesStopGenerator::EnginesStopGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "engines/stop.t", QList<Binding *>()
			<< Binding::createConverting("@@Port@@", "Ports", customizer.factory()->portNameConverter())
			, parent)
{
}
