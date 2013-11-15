#include "russianCGeneratorCustomizer.h"

using namespace qReal::robots::generators::russianC;

RussianCGeneratorCustomizer::RussianCGeneratorCustomizer(qrRepo::RepoApi const &repo
		, ErrorReporterInterface &errorReporter)
	: mFactory(new RussianCGeneratorFactory(repo, errorReporter))
{
}

qReal::robots::generators::GeneratorFactoryBase *RussianCGeneratorCustomizer::factory()
{
	return mFactory;
}
