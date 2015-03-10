#include "nxtRussianCMasterGenerator.h"

using namespace nxt::russianC;

NxtRussianCMasterGenerator::NxtRussianCMasterGenerator(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, const utils::ParserErrorReporter &parserErrorReporter
		, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage
		, const qReal::Id &diagramId
		, const QString &generatorName)
	: NxtMasterGeneratorBase(repo, errorReporter, parserErrorReporter
			, robotModelManager, textLanguage, diagramId, generatorName)
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
