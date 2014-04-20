#include "trikEnginesGenerator.h"

#include <generatorBase/parts/engines.h>
#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;
using namespace generatorBase::simple;

// TODO: implement servos generation
TrikEnginesGenerator::TrikEnginesGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QString const &engineType
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, engineType == "EnginesBackward"
					? "engines/power/backward.t"
					: "engines/power/forward.t"
			, QList<Binding *>()
					<< Binding::createMultiTarget("@@PORT@@", "Ports", customizer.factory()->enginesConverter())
					<< Binding::createConverting("@@POWER@@", "Power", customizer.factory()->intPropertyConverter())
			, parent)
{
	mCustomizer.factory()->engines()->registerUsageOnPorts(repo.property(id, "Ports").toString());
}
