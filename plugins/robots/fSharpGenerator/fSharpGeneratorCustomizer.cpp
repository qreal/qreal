#include "fSharpGeneratorCustomizer.h"

using namespace qReal::robots::generators::fSharp;

FSharpGeneratorCustomizer::FSharpGeneratorCustomizer(qrRepo::RepoApi const &repo
        , ErrorReporterInterface &errorReporter)
    : mFactory(repo, errorReporter)
{
}

qReal::robots::generators::GeneratorFactoryBase *FSharpGeneratorCustomizer::factory()
{
    return &mFactory;
}
