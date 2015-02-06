#pragma once

#include <generatorBase/masterGeneratorBase.h>

#include "trikGeneratorBase/trikGeneratorBaseDeclSpec.h"

namespace trik {

/// Master generator base implementation for every generator from TRIK kit
class ROBOTS_TRIK_GENERATOR_BASE_EXPORT TrikMasterGeneratorBase : public generatorBase::MasterGeneratorBase
{
public:
	TrikMasterGeneratorBase(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, const utils::ParserErrorReporter &parserErrorReporter
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, qrtext::LanguageToolboxInterface &textLanguage
			, qReal::Id const &diagramId
			, QString const &generatorName);

protected:
	generatorBase::GeneratorCustomizer *createCustomizer() override;

private:
	QString const mGeneratorName;
};

}
