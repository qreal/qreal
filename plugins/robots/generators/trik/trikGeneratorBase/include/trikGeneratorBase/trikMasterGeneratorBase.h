#pragma once

#include <generatorBase/masterGeneratorBase.h>

namespace trik {

/// Master generator base implementation for every generator from TRIK kit
class TrikMasterGeneratorBase : public generatorBase::MasterGeneratorBase
{
public:
	TrikMasterGeneratorBase(qrRepo::RepoApi const &repo
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
