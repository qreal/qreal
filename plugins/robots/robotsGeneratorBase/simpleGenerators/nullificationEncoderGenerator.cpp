#include "nullificationEncoderGenerator.h"
#include "../generatorCustomizer.h"

using namespace qReal::robots::generators::simple;

NullificationEncoderGenerator::NullificationEncoderGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "engines/nullifyEncoder.t", QList<Binding *>()
			<< Binding::createMultiTarget("@@PORT@@", "Ports", customizer.factory()->enginesConverter())
			, parent)
{
}
