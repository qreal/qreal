#include "nxtOsekGeneratorFactory.h"

using namespace qReal::robots::generators::nxtOsek;

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
