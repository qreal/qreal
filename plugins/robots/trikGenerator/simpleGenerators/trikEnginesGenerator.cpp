#include "trikEnginesGenerator.h"
#include <generatorCustomizer.h>

using namespace qReal::robots::generators::simple;

TrikEnginesGenerator::TrikEnginesGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QString const &engineType
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, engineType == "EnginesBackward"
					? QString("engines/") + (repo.property(id, "powerMotors").toBool() ? "power" : "servo") + "/backward.t"
					: QString("engines/") + (repo.property(id, "powerMotors").toBool() ? "power" : "servo") + "/forward.t"
			, QList<Binding *>()
					<< Binding::createMultiTarget("@@PORT@@", "Ports", customizer.factory()->enginesConverter())
					<< Binding::createConverting("@@POWER@@", "Power", customizer.factory()->intPropertyConverter())
			, parent)
{
	mCustomizer.factory()->engines()->registerUsageOnPorts(repo.property(id, "Ports").toString());
}
