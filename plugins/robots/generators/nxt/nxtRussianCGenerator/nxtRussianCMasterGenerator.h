#pragma once

#include <generatorBase/masterGeneratorBase.h>

namespace russianC {

class NxtRussianCMasterGenerator : public generatorBase::MasterGeneratorBase
{
public:
	NxtRussianCMasterGenerator(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, qrtext::LanguageToolboxInterface &textLanguage
			, qReal::Id const &diagramId);

protected:
	generatorBase::GeneratorCustomizer *createCustomizer() override;
	QString targetPath() override;
	bool supportsGotoGeneration() const override;

	void afterGeneration() override;

private:
	void saveImages(QString const &projectDir);

	int mCurInitialNodeNumber;
};

}
