#include "enginesGenerator.h"

#include "generatorBase/generatorCustomizer.h"
#include "generatorBase/parts/engines.h"

using namespace generatorBase::simple;
using namespace qReal;

EnginesGenerator::EnginesGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QString const &engineType
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, engineType.contains("EnginesBackward")
					? "engines/backward.t"
					: "engines/forward.t"
			, { Binding::createMultiTarget("@@PORT@@", "Ports", customizer.factory()->enginesConverter())
					, Binding::createConverting("@@POWER@@", "Power"
							, customizer.factory()->intPropertyConverter(id, "Power"))
					, Binding::createConverting("@@BREAK_MODE@@", "Mode", customizer.factory()->breakModeConverter())
			}, parent)
{
	mCustomizer.factory()->engines()->registerUsageOnPorts(repo.property(id, "Ports").toString());
}
