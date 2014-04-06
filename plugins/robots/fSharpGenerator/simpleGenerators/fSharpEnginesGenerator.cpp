#include "fSharpEnginesGenerator.h"

#include <generatorCustomizer.h>
#include "fSharpGeneratorFactory.h"

using namespace qReal::robots::generators::simple;

FSharpEnginesGenerator::FSharpEnginesGenerator(qrRepo::RepoApi const &repo
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
							, dynamic_cast<fSharp::FSharpGeneratorFactory *>(customizer.factory())->
									enginesConverter(repo.property(id, "powerMotors").toBool()))
					<< Binding::createConverting("@@POWER@@", "Power", customizer.factory()->intPropertyConverter())
			, parent)
{
	mCustomizer.factory()->engines()->registerUsageOnPorts(repo.property(id, "Ports").toString());
}
