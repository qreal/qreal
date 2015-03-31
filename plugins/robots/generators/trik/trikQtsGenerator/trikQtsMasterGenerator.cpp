#include "trikQtsMasterGenerator.h"

#include "trikQtsControlFlowValidator.h"

using namespace trik::qts;

TrikQtsMasterGenerator::TrikQtsMasterGenerator(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, const utils::ParserErrorReporter &parserErrorReporter
		, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage
		, const qReal::Id &diagramId
		, const QString &generatorName)
	: TrikMasterGeneratorBase(repo, errorReporter, parserErrorReporter, robotModelManager, textLanguage
			, diagramId, generatorName)
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

generatorBase::PrimaryControlFlowValidator *TrikQtsMasterGenerator::createValidator()
{
	return new TrikQtsControlFlowValidator(mRepo, mErrorReporter, *mCustomizer, this);
}
