#include "nullificationEncoderGenerator.h"
#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;
using namespace qReal;

NullificationEncoderGenerator::NullificationEncoderGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "engines/nullifyEncoder.t", QList<Binding *>()
			<< Binding::createMultiTarget("@@PORT@@", "Ports", customizer.factory()->enginesConverter())
			, parent)
{
}
