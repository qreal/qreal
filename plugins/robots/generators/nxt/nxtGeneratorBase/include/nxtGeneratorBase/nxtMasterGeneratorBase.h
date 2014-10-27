#pragma once

#include <generatorBase/masterGeneratorBase.h>

namespace nxt {

class NxtMasterGeneratorBase : public generatorBase::MasterGeneratorBase
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
	void afterGeneration() override;

private:
	void saveImages(QString const &projectDir);

	QString const mGeneratorName;
};

}
