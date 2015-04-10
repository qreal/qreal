#include "enginesStopGenerator.h"

#include <generatorBase/generatorCustomizer.h>
#include "src/ev3GeneratorCustomizer.h"

using namespace ev3::simple;
using namespace generatorBase::simple;

EnginesStopGenerator::EnginesStopGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "engines/stop.t", QList<Binding *>()
			<< Binding::createConverting("@@Port@@", "Ports", static_cast<Ev3GeneratorFactory *>(customizer.factory())->outputPortNameConverter())
			, parent)
{
}
