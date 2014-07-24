#include "fSharpEnginesStopGenerator.h"

#include <generatorBase/generatorCustomizer.h>
#include "fSharpGeneratorFactory.h"

using namespace fSharp::simple;
using namespace generatorBase::simple;

FSharpEnginesStopGenerator::FSharpEnginesStopGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "engines/stop.t", QList<Binding *>()
			<< Binding::createMultiTarget("@@PORT@@", "Ports"
					, dynamic_cast<fSharp::FSharpGeneratorFactory *>(customizer.factory())->enginesConverter())
			, parent)
{
}
