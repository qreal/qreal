#pragma once

#include <generatorBase/masterGeneratorBase.h>

#include "nxtGeneratorBase/nxtGeneratorBaseDeclSpec.h"

namespace nxt {

/// Master generator base implementation for every generator from NXT kit
class ROBOTS_NXT_GENERATOR_BASE_EXPORT NxtMasterGeneratorBase : public generatorBase::MasterGeneratorBase
{
public:
	NxtMasterGeneratorBase(const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, const utils::ParserErrorReporter &parserErrorReporter
			, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
			, qrtext::LanguageToolboxInterface &textLanguage
			, const qReal::Id &diagramId
			, const QString &generatorName);

protected:
	generatorBase::GeneratorCustomizer *createCustomizer() override;
	void beforeGeneration() override;
	void processGeneratedCode(QString &code) override;
	void afterGeneration() override;

private:
	void saveImages(const QString &projectDir);

	const QString mGeneratorName;
};

}
