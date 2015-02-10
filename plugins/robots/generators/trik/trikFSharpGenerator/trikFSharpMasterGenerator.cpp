#include "trikFSharpMasterGenerator.h"

#include <qrutils/outFile.h>
#include <qrutils/stringUtils.h>

using namespace trik::fSharp;

TrikFSharpMasterGenerator::TrikFSharpMasterGenerator(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, const utils::ParserErrorReporter &parserErrorReporter
		, const interpreterBase::robotModel::RobotModelManagerInterface &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage
		, const qReal::Id &diagramId
		, const QString &generatorName)
	: TrikMasterGeneratorBase(repo, errorReporter, parserErrorReporter, robotModelManager, textLanguage, diagramId
			, generatorName)
{
}

QString TrikFSharpMasterGenerator::targetPath()
{
	return QString("%1/%2.fs").arg(mProjectDir, mProjectName);
}

bool TrikFSharpMasterGenerator::supportsGotoGeneration() const
{
	return false;
}
