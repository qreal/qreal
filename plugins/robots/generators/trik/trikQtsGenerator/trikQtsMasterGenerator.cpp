#include "trikQtsMasterGenerator.h"
#include "trikQtsGeneratorCustomizer.h"

using namespace trik;

TrikQtsMasterGenerator::TrikQtsMasterGenerator(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage
		, qReal::Id const &diagramId)
	: MasterGeneratorBase(repo, errorReporter, robotModelManager, textLanguage, diagramId)
{
}

generatorBase::GeneratorCustomizer *TrikQtsMasterGenerator::createCustomizer()
{
	return new TrikQtsGeneratorCustomizer(mRepo, mErrorReporter, mRobotModelManager, *createLuaProcessor());
}

QString TrikQtsMasterGenerator::targetPath()
{
	return QString("%1/%2.qts").arg(mProjectDir, mProjectName);
}

bool TrikQtsMasterGenerator::supportsGotoGeneration() const
{
	return false;
}
