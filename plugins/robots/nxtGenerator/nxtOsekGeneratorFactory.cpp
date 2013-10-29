#include "nxtOsekGeneratorFactory.h"

#include "converters/nxtStringPropertyConverter.h"

using namespace qReal::robots::generators;
using namespace nxtOsek;

NxtOsekGeneratorFactory::NxtOsekGeneratorFactory(qrRepo::RepoApi const &repo
		, ErrorReporterInterface &errorReporter)
	: GeneratorFactoryBase(repo, errorReporter)
{
}

NxtOsekGeneratorFactory::~NxtOsekGeneratorFactory()
{
}

QString NxtOsekGeneratorFactory::pathToTemplates() const
{
	return ":/nxtOSEK/templates";
}

simple::Binding::ConverterInterface *NxtOsekGeneratorFactory::stringPropertyConverter() const
{
	return new converters::NxtStringPropertyConverter(*mVariables, *mSubprograms);
}
