#pragma once

#include <generatorBase/masterGeneratorBase.h>

#include "trikGeneratorBase/trikGeneratorBaseDeclSpec.h"

namespace trik {

/// Master generator base implementation for every generator from TRIK kit
class ROBOTS_TRIK_GENERATOR_BASE_EXPORT TrikMasterGeneratorBase : public generatorBase::MasterGeneratorBase
{
public:
	TrikMasterGeneratorBase(const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, const utils::ParserErrorReporter &parserErrorReporter
			, const interpreterBase::robotModel::RobotModelManagerInterface &robotModelManager
			, qrtext::LanguageToolboxInterface &textLanguage
			, const qReal::Id &diagramId
			, const QString &generatorName);

protected:
	generatorBase::GeneratorCustomizer *createCustomizer() override;

private:
	const QString mGeneratorName;
};

}
