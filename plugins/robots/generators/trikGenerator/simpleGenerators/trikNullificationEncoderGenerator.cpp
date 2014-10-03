#include "trikNullificationEncoderGenerator.h"

#include <generatorBase/generatorCustomizer.h>
#include "trikGeneratorFactory.h"

using namespace trik::simple;
using namespace generatorBase::simple;

TrikNullificationEncoderGenerator::TrikNullificationEncoderGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "engines/nullifyEncoder.t", QList<Binding *>()
			<< Binding::createMultiTarget("@@PORT@@", "Ports"
					, dynamic_cast<trik::TrikGeneratorFactory *>(customizer.factory())->encodersConverter())
			, parent)
{
}
