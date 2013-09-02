#include "nxtOsekGeneratorFactory.h"

using namespace qReal::robots::generators::nxtOsek;

NxtOsekGeneratorFactory::NxtOsekGeneratorFactory(LogicalModelAssistInterface const &model)
	: GeneratorFactoryBase(model)
{
}

NxtOsekGeneratorFactory::~NxtOsekGeneratorFactory()
{
}

QString NxtOsekGeneratorFactory::pathToTemplates() const
{
	return ":/templates/nxtOSEK";
}
