#include "russianCGeneratorCustomizer.h"

using namespace russianC;

RussianCGeneratorCustomizer::RussianCGeneratorCustomizer(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter)
	: mFactory(new RussianCGeneratorFactory(repo, errorReporter))
{
}

generatorBase::GeneratorFactoryBase *RussianCGeneratorCustomizer::factory()
{
	return mFactory;
}
