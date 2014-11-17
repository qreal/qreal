#include "subprogramsSimpleGenerator.h"

#include "generatorBase/generatorCustomizer.h"
#include "generatorBase/parts/subprograms.h"

using namespace generatorBase::simple;
using namespace qReal;

SubprogramsSimpleGenerator::SubprogramsSimpleGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "subprograms/subprogramCall.t", QList<Binding *>()
			<< Binding::createConverting("@@NAME@@", "name", customizer.factory()->nameNormalizerConverter())
			, parent)
{
}
