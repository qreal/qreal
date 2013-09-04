#include "nxtOsekGeneratorCustomizer.h"

using namespace qReal::robots::generators::nxtOsek;

NxtOsekGeneratorCustomizer::NxtOsekGeneratorCustomizer(qrRepo::RepoApi const &repo)
	: mFactory(new NxtOsekGeneratorFactory(repo))
{
}

qReal::robots::generators::GeneratorFactoryBase *NxtOsekGeneratorCustomizer::factory()
{
	return mFactory;
}
