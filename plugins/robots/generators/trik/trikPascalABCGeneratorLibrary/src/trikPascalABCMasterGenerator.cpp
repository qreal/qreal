#include "trikPascalABCMasterGenerator.h"

#include "trikPascalABCControlFlowValidator.h"

using namespace trik::pascalABC;

TrikPascalABCMasterGenerator::TrikPascalABCMasterGenerator(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, const utils::ParserErrorReporter &parserErrorReporter
		, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage
		, const qReal::Id &diagramId
		, const QStringList &pathsToTemplates)
	: TrikMasterGeneratorBase(repo, errorReporter, parserErrorReporter, robotModelManager, textLanguage
			, diagramId, pathsToTemplates)
{
}

QString TrikPascalABCMasterGenerator::targetPath()
{
	return QString("%1/%2.pas").arg(mProjectDir, mProjectName);
}

bool TrikPascalABCMasterGenerator::supportsGotoGeneration() const
{
	return false;
}

generatorBase::PrimaryControlFlowValidator *TrikPascalABCMasterGenerator::createValidator()
{
	return new TrikPascalABCControlFlowValidator(mRepo, mErrorReporter, *mCustomizer, this);
}
