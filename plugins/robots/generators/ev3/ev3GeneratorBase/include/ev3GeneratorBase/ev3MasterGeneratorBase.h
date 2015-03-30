#pragma once

#include <generatorBase/masterGeneratorBase.h>

#include "ev3GeneratorBase/ev3GeneratorBaseDeclSpec.h"

namespace ev3 {

/// Master generator base implementation for every generator from TRIK kit
class ROBOTS_EV3_GENERATOR_BASE_EXPORT Ev3MasterGeneratorBase : public generatorBase::MasterGeneratorBase
{
public:
	Ev3MasterGeneratorBase(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
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
