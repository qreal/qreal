#pragma once

#include <generatorBase/masterGeneratorBase.h>

#include "nxtGeneratorBase/nxtGeneratorBaseDeclSpec.h"

namespace nxt {

/// Master generator base implementation for every generator from NXT kit
class ROBOTS_NXT_GENERATOR_BASE_EXPORT NxtMasterGeneratorBase : public generatorBase::MasterGeneratorBase
{
public:
	NxtMasterGeneratorBase(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, qrtext::LanguageToolboxInterface &textLanguage
			, qReal::Id const &diagramId
			, QString const &generatorName);

protected:
	generatorBase::GeneratorCustomizer *createCustomizer() override;
	void beforeGeneration() override;
	void processGeneratedCode(QString &code) override;
	void afterGeneration() override;

private:
	void saveImages(QString const &projectDir);

	QString const mGeneratorName;
};

}
