#include "subprogramsSimpleGenerator.h"
#include "../generatorCustomizer.h"

using namespace qReal::robots::generators::simple;

SubprogramsSimpleGenerator::SubprogramsSimpleGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "subprograms/subprogramCall.t", QList<Binding *>()
			<< Binding::createConverting("@@NAME@@", "name", customizer.factory()->nameNormalizerConverter())
			, parent)
{
}

QString SubprogramsSimpleGenerator::generate()
{
	mCustomizer.factory()->subprograms()->usageFound(mId);
	return BindingGenerator::generate();
}
