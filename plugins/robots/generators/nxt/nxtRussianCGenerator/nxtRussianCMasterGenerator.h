#pragma once

#include <nxtGeneratorBase/nxtMasterGeneratorBase.h>

namespace nxt {
namespace russianC {

class NxtRussianCMasterGenerator : public NxtMasterGeneratorBase
{
public:
	NxtRussianCMasterGenerator(const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, const utils::ParserErrorReporter &parserErrorReporter
			, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
			, qrtext::LanguageToolboxInterface &textLanguage
			, const qReal::Id &diagramId
			, const QString &generatorName);

protected:
	QString targetPath() override;
	bool supportsGotoGeneration() const override;
};

}
}
