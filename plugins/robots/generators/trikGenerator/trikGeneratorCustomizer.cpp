#include "trikGeneratorCustomizer.h"

using namespace trik;

TrikGeneratorCustomizer::TrikGeneratorCustomizer(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter)
	: mFactory(repo, errorReporter)
{
}

generatorBase::GeneratorFactoryBase *TrikGeneratorCustomizer::factory()
{
	return &mFactory;
}
