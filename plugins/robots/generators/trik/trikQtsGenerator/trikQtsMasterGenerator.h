#pragma once

#include <trikGeneratorBase/trikMasterGeneratorBase.h>

namespace trik {
namespace qts {

/// Master generator implementation for generator into QtScript for TRIK platform
class TrikQtsMasterGenerator : public TrikMasterGeneratorBase
{
public:
	TrikQtsMasterGenerator(const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, const utils::ParserErrorReporter &parserErrorReporter
			, const interpreterBase::robotModel::RobotModelManagerInterface &robotModelManager
			, qrtext::LanguageToolboxInterface &textLanguage
			, const qReal::Id &diagramId
			, const QString &generatorName);

protected:
	QString targetPath() override;
	bool supportsGotoGeneration() const override;
};

}
}
