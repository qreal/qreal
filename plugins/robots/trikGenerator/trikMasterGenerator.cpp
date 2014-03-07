#include "trikMasterGenerator.h"
#include "trikGeneratorCustomizer.h"

using namespace qReal::robots::generators::trik;

TrikMasterGenerator::TrikMasterGenerator(qrRepo::RepoApi const &repo
		, ErrorReporterInterface &errorReporter
		, Id const &diagramId)
	: MasterGeneratorBase(repo, errorReporter, diagramId)
{
}

qReal::robots::generators::GeneratorCustomizer *TrikMasterGenerator::createCustomizer()
{
	return new TrikGeneratorCustomizer(mRepo, mErrorReporter);
}

QString TrikMasterGenerator::targetPath()
{
	return QString("%1/%2.qts").arg(mProjectDir, mProjectName);
}
