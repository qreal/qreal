#pragma once

#include <trikGeneratorBase/trikMasterGeneratorBase.h>

namespace trik {
namespace pascalABC {

/// Master generator implementation for generator into PascalABC for TRIK platform
class TrikPascalABCMasterGenerator : public TrikMasterGeneratorBase
{
public:
	TrikPascalABCMasterGenerator(const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, const utils::ParserErrorReporter &parserErrorReporter
			, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
			, qrtext::LanguageToolboxInterface &textLanguage
			, const qReal::Id &diagramId
			, const QStringList &pathsToTemplates);

protected:
	QString targetPath() override;
	bool supportsGotoGeneration() const override;

	generatorBase::PrimaryControlFlowValidator *createValidator() override;
};

}
}


