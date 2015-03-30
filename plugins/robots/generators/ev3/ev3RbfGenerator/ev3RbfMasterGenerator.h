#pragma once

#include <ev3GeneratorBase/ev3MasterGeneratorBase.h>

namespace ev3 {
namespace rbf {

class Ev3RbfMasterGenerator : public Ev3MasterGeneratorBase
{
public:
	Ev3RbfMasterGenerator(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, qrtext::LanguageToolboxInterface &textLanguage
			, qReal::Id const &diagramId
			, QString const &generatorName);

//	void generateOilAndMakeFiles();

//protected:
//	QString targetPath() override;
//	bool supportsGotoGeneration() const override;

//	void afterGeneration() override;

//private:
//	void generateOilFile(QString const &projectName, QString const &projectDir);
//	QString generateOilTask(QString const &taskName);
//	void generateMakeFile(QString const &projectName, QString const &projectDir);
};

}
}

