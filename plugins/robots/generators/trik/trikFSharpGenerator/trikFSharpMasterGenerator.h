#pragma once

#include <trikGeneratorBase/trikMasterGeneratorBase.h>

namespace trik {
namespace fSharp {

/// Master generator implementation for TRIK platform
class TrikFSharpMasterGenerator : public TrikMasterGeneratorBase
{
public:
	TrikFSharpMasterGenerator(const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, const utils::ParserErrorReporter &parserErrorReporter
			, const interpreterBase::robotModel::RobotModelManagerInterface &robotModelManager
			, qrtext::LanguageToolboxInterface &textLanguage
			, const qReal::Id &diagramId
			, const QString &generatorName);

protected:
	QString targetPath() override;
	bool supportsGotoGeneration() const override;

	generatorBase::PrimaryControlFlowValidator *createValidator() override;
};

}
}
