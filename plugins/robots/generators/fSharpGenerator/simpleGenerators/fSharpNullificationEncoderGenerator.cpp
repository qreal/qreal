#include "fSharpNullificationEncoderGenerator.h"

#include <generatorBase/generatorCustomizer.h>
#include "fSharpGeneratorFactory.h"

using namespace fSharp::simple;
using namespace generatorBase::simple;

FSharpNullificationEncoderGenerator::FSharpNullificationEncoderGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "engines/nullifyEncoder.t", QList<Binding *>()
			<< Binding::createMultiTarget("@@PORT@@", "Ports"
					, dynamic_cast<fSharp::FSharpGeneratorFactory *>(customizer.factory())->encodersConverter())
			, parent)
{
}
