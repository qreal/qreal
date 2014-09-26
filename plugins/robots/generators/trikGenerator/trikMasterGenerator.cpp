#include "trikMasterGenerator.h"
#include "trikGeneratorCustomizer.h"

using namespace trik;

TrikMasterGenerator::TrikMasterGenerator(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage
		, qReal::Id const &diagramId)
	: MasterGeneratorBase(repo, errorReporter, robotModelManager, textLanguage, diagramId)
{
}

generatorBase::GeneratorCustomizer *TrikMasterGenerator::createCustomizer()
{
	return new TrikGeneratorCustomizer(mRepo, mErrorReporter, mRobotModelManager, mTextLanguage);
}

QString TrikMasterGenerator::targetPath()
{
	return QString("%1/%2.qts").arg(mProjectDir, mProjectName);
}

bool TrikMasterGenerator::supportsGotoGeneration() const
{
	return false;
}
