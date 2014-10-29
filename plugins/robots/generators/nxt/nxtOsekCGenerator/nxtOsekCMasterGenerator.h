#pragma once

#include <nxtGeneratorBase/nxtMasterGeneratorBase.h>

namespace nxt {
namespace osekC {

class NxtOsekCMasterGenerator : public NxtMasterGeneratorBase
{
public:
	NxtOsekCMasterGenerator(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, qrtext::LanguageToolboxInterface &textLanguage
			, qReal::Id const &diagramId
			, QString const &generatorName);

	void generateOilAndMakeFiles();

protected:
	QString targetPath() override;
	bool supportsGotoGeneration() const override;

	void afterGeneration() override;

private:
	void generateOilFile(QString const &projectName, QString const &projectDir);
	QString generateOilTask(QString const &taskName);
	void generateMakeFile(QString const &projectName, QString const &projectDir);
};

}
}
