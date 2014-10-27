#include "nxtRussianCMasterGenerator.h"

#include <generatorBase/parts/images.h>

using namespace nxt::russianC;

NxtRussianCMasterGenerator::NxtRussianCMasterGenerator(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage
		, qReal::Id const &diagramId
		, QString const &generatorName)
	: NxtMasterGeneratorBase(repo, errorReporter, robotModelManager, textLanguage, diagramId, generatorName)
{
}

QString NxtRussianCMasterGenerator::targetPath()
{
	return QString::fromUtf8("%1/%2.си").arg(mProjectDir, mProjectName);
}

bool NxtRussianCMasterGenerator::supportsGotoGeneration() const
{
	return false;
}
