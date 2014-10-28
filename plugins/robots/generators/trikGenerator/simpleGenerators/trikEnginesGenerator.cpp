#include "trikEnginesGenerator.h"

#include <generatorBase/generatorCustomizer.h>
#include <generatorBase/parts/engines.h>
#include "trikGeneratorFactory.h"

using namespace trik::simple;
using namespace generatorBase::simple;

TrikEnginesGenerator::TrikEnginesGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QString const &engineType
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, engineType.contains("EnginesBackward") ? "engines/backward.t" : "engines/forward.t"
			, { Binding::createMultiTarget("@@PORT@@", "Ports", customizer.factory()->enginesConverter())
					, Binding::createConverting("@@POWER@@", "Power"
							, customizer.factory()->intPropertyConverter(id, "Power"))
			}, parent)
{
	// AngularServo is just another block for EnginesForward, since it is controlled the same way as radial servo
	// or power motor.
	mCustomizer.factory()->engines()->registerUsageOnPorts(repo.property(id, "Ports").toString());
}
