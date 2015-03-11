#pragma once

#include <nxtGeneratorBase/nxtMasterGeneratorBase.h>

namespace nxt {
namespace osekC {

class NxtOsekCMasterGenerator : public NxtMasterGeneratorBase
{
public:
	NxtOsekCMasterGenerator(const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, const utils::ParserErrorReporter &parserErrorReporter
			, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
			, qrtext::LanguageToolboxInterface &textLanguage
			, const qReal::Id &diagramId
			, const QString &generatorName);

	void generateOilAndMakeFiles();

protected:
	QString targetPath() override;
	bool supportsGotoGeneration() const override;

	void afterGeneration() override;

private:
	void generateOilFile(const QString &projectName, const QString &projectDir);
	QString generateOilTask(const QString &taskName);
	void generateMakeFile(const QString &projectName, const QString &projectDir);
};

}
}
