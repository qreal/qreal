#pragma once

#include <generatorBase/masterGeneratorBase.h>

namespace nxtOsek {

class NxtOsekMasterGenerator : public generatorBase::MasterGeneratorBase
{
public:
	NxtOsekMasterGenerator(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, qrtext::LanguageToolboxInterface &textLanguage
			, qReal::Id const &diagramId);

	void generateOilAndMakeFiles();

protected:
	generatorBase::GeneratorCustomizer *createCustomizer() override;
	QString targetPath() override;
	bool supportsGotoGeneration() const override;

	void afterGeneration() override;

private:
	void generateOilFile(QString const &projectName, QString const &projectDir);
	QString generateOilTask(QString const &taskName);
	void generateMakeFile(QString const &projectName, QString const &projectDir);
	void saveImages(QString const &projectDir);
};

}
