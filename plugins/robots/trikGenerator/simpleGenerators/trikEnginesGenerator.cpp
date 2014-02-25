#include "trikEnginesGenerator.h"

#include <generatorCustomizer.h>
#include "trikGeneratorFactory.h"

using namespace qReal::robots::generators::simple;

TrikEnginesGenerator::TrikEnginesGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QString const &engineType
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, engineType == "EnginesBackward"
					? "engines/backward.t"
					: "engines/forward.t"
			, QList<Binding *>()
					<< Binding::createMultiTarget("@@PORT@@", "Ports"
							, dynamic_cast<trik::TrikGeneratorFactory *>(customizer.factory())->
									enginesConverter(repo.property(id, "powerMotors").toBool()))
					<< Binding::createConverting("@@POWER@@", "Power", customizer.factory()->intPropertyConverter())
			, parent)
{
	mCustomizer.factory()->engines()->registerUsageOnPorts(repo.property(id, "Ports").toString());
}
