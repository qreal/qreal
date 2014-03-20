#include "nxtOsekGeneratorCustomizer.h"

using namespace qReal::robots::generators::nxtOsek;

NxtOsekGeneratorCustomizer::NxtOsekGeneratorCustomizer(qrRepo::RepoApi const &repo
		, ErrorReporterInterface &errorReporter)
	: mFactory(repo, errorReporter)
{
}

qReal::robots::generators::GeneratorFactoryBase *NxtOsekGeneratorCustomizer::factory()
{
	return &mFactory;
}
