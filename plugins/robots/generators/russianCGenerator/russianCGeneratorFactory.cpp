#include "russianCGeneratorFactory.h"

#include "converters/russianCStringPropertyConverter.h"

using namespace russianC;

RussianCGeneratorFactory::RussianCGeneratorFactory(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter)
	: generatorBase::GeneratorFactoryBase(repo, errorReporter)
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
