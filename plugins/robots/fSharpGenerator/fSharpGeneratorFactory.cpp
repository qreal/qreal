#include "fSharpGeneratorFactory.h"


using namespace qReal::robots::generators;
using namespace fSharp;

FSharpGeneratorFactory::FSharpGeneratorFactory(qrRepo::RepoApi const &repo
        , ErrorReporterInterface &errorReporter)
    : GeneratorFactoryBase(repo, errorReporter)
{
}

FSharpGeneratorFactory::~FSharpGeneratorFactory()
{
}

QString FSharpGeneratorFactory::pathToTemplates() const
{
    return ":/russianC/templates";
}

simple::Binding::ConverterInterface *FSharpGeneratorFactory::stringPropertyConverter() const
{
    return new converters::RussianCStringPropertyConverter(*mVariables);
}
