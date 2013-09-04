#include "nxtOsekGeneratorFactory.h"

using namespace qReal::robots::generators::nxtOsek;

NxtOsekGeneratorFactory::NxtOsekGeneratorFactory(qrRepo::RepoApi const &repo)
	: GeneratorFactoryBase(repo)
{
}

NxtOsekGeneratorFactory::~NxtOsekGeneratorFactory()
{
}

QString NxtOsekGeneratorFactory::pathToTemplates() const
{
	return ":/nxtOSEK/templates";
}
