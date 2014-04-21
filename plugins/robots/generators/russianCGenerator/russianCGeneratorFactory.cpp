#include "russianCGeneratorFactory.h"

#include "converters/russianCStringPropertyConverter.h"

using namespace russianC;

RussianCGeneratorFactory::RussianCGeneratorFactory(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager)
	: generatorBase::GeneratorFactoryBase(repo, errorReporter, robotModelManager)
{
}

RussianCGeneratorFactory::~RussianCGeneratorFactory()
{
}

QString RussianCGeneratorFactory::pathToTemplates() const
{
	return ":/russianC/templates";
}

generatorBase::simple::Binding::ConverterInterface *RussianCGeneratorFactory::stringPropertyConverter() const
{
	return new converters::RussianCStringPropertyConverter(*mVariables);
}
