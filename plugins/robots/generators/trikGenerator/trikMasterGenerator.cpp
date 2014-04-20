#include "trikMasterGenerator.h"
#include "trikGeneratorCustomizer.h"

using namespace trik;

TrikMasterGenerator::TrikMasterGenerator(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, qReal::Id const &diagramId)
	: MasterGeneratorBase(repo, errorReporter, diagramId)
{
}

generatorBase::GeneratorCustomizer *TrikMasterGenerator::createCustomizer()
{
	return new TrikGeneratorCustomizer(mRepo, mErrorReporter);
}

QString TrikMasterGenerator::targetPath()
{
	return QString("%1/%2.qts").arg(mProjectDir, mProjectName);
}

bool TrikMasterGenerator::supportsGotoGeneration() const
{
	return false;
}
