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
