#include "nxtOsekGeneratorCustomizer.h"

using namespace nxtOsek;

NxtOsekGeneratorCustomizer::NxtOsekGeneratorCustomizer(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter)
	: mFactory(repo, errorReporter)
{
}

generatorBase::GeneratorFactoryBase *NxtOsekGeneratorCustomizer::factory()
{
	return &mFactory;
}
