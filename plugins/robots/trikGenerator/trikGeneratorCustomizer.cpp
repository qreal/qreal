#include "trikGeneratorCustomizer.h"

using namespace qReal::robots::generators::trik;

TrikGeneratorCustomizer::TrikGeneratorCustomizer(qrRepo::RepoApi const &repo
		, ErrorReporterInterface &errorReporter)
	: mFactory(new TrikGeneratorFactory(repo, errorReporter))
{
}

qReal::robots::generators::GeneratorFactoryBase *TrikGeneratorCustomizer::factory()
{
	return mFactory;
}
