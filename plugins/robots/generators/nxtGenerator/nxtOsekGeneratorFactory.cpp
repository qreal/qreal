#include "nxtOsekGeneratorFactory.h"

#include "converters/nxtStringPropertyConverter.h"

using namespace nxtOsek;

NxtOsekGeneratorFactory::NxtOsekGeneratorFactory(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager)
	: GeneratorFactoryBase(repo, errorReporter, robotModelManager)
{
}

NxtOsekGeneratorFactory::~NxtOsekGeneratorFactory()
{
}

QString NxtOsekGeneratorFactory::pathToTemplates() const
{
	return ":/nxtOSEK/templates";
}

generatorBase::simple::Binding::ConverterInterface *NxtOsekGeneratorFactory::stringPropertyConverter() const
{
	return new converters::NxtStringPropertyConverter(*mVariables, *mSubprograms);
}
