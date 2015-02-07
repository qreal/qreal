#pragma once

#include <nxtGeneratorBase/nxtMasterGeneratorBase.h>

namespace nxt {
namespace russianC {

class NxtRussianCMasterGenerator : public NxtMasterGeneratorBase
{
public:
	NxtRussianCMasterGenerator(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, const utils::ParserErrorReporter &parserErrorReporter
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, qrtext::LanguageToolboxInterface &textLanguage
			, qReal::Id const &diagramId
			, QString const &generatorName);

protected:
	QString targetPath() override;
	bool supportsGotoGeneration() const override;
};

}
}
