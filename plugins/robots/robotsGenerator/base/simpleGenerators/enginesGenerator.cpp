#include "enginesGenerator.h"
#include "../generatorCustomizer.h"

using namespace qReal::robots::generators::simple;

EnginesGenerator::EnginesGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QString const &engineType
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, engineType == "EnginesBackward"
					? "engines/enginesBackward.t"
					: "engines/enginesForward.t"
			, QList<Binding *>()
					<< Binding::createMultiTarget("@@PORT@@", "Ports", customizer.factory()->enginesConverter())
					<< Binding::createConverting("@@POWER@@", "Power", customizer.factory()->intPropertyConverter())
					<< Binding::createConverting("@@BREAK_MODE@@", "Mode", customizer.factory()->breakModeConverter())
			, parent)
{
	mCustomizer.factory()->engines()->registerUsageOnPorts(repo.property(id, "Ports").toString());
}
