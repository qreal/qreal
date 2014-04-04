#include "fSharpEnginesStopGenerator.h"

#include <generatorCustomizer.h>
#include "fSharpGeneratorFactory.h"

using namespace qReal::robots::generators::simple;

FSharpEnginesStopGenerator::FSharpEnginesStopGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "engines/stop.t", QList<Binding *>()
			<< Binding::createMultiTarget("@@PORT@@", "Ports"
                    , dynamic_cast<fSharp::FSharpGeneratorFactory *>(customizer.factory())->
							enginesConverter(repo.property(id, "powerMotors").toBool()))
			, parent)
{
}
