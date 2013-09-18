#include "trikGeneratorFactory.h"

using namespace qReal::robots::generators::trik;

TrikGeneratorFactory::TrikGeneratorFactory(qrRepo::RepoApi const &repo
		, ErrorReporterInterface &errorReporter)
	: GeneratorFactoryBase(repo, errorReporter)
{
}

TrikGeneratorFactory::~TrikGeneratorFactory()
{
}

QString TrikGeneratorFactory::pathToTemplates() const
{
	return ":/trik/templates";
}
