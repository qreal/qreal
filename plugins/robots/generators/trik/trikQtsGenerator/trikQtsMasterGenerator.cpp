#include "trikQtsMasterGenerator.h"

using namespace trik::qts;

TrikQtsMasterGenerator::TrikQtsMasterGenerator(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage
		, qReal::Id const &diagramId
		, QString const &generatorName)
	: TrikMasterGeneratorBase(repo, errorReporter, robotModelManager, textLanguage, diagramId, generatorName)
{
}

QString TrikQtsMasterGenerator::targetPath()
{
	return QString("%1/%2.qts").arg(mProjectDir, mProjectName);
}

bool TrikQtsMasterGenerator::supportsGotoGeneration() const
{
	return false;
}
