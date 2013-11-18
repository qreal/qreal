#include "russianCGeneratorFactory.h"

#include "converters/russianCStringPropertyConverter.h"

using namespace qReal::robots::generators;
using namespace russianC;

RussianCGeneratorFactory::RussianCGeneratorFactory(qrRepo::RepoApi const &repo
		, ErrorReporterInterface &errorReporter)
	: GeneratorFactoryBase(repo, errorReporter)
{
}

RussianCGeneratorFactory::~RussianCGeneratorFactory()
{
}

QString RussianCGeneratorFactory::pathToTemplates() const
{
	return ":/russianC/templates";
}

simple::Binding::ConverterInterface *RussianCGeneratorFactory::stringPropertyConverter() const
{
	return new converters::RussianCStringPropertyConverter(*mVariables);
}
