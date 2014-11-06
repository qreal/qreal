#include "trikFSharpMasterGenerator.h"

#include <qrutils/outFile.h>
#include <qrutils/stringUtils.h>

using namespace trik::fSharp;

TrikFSharpMasterGenerator::TrikFSharpMasterGenerator(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage
		, qReal::Id const &diagramId
		, QString const &generatorName)
	: TrikMasterGeneratorBase(repo, errorReporter, robotModelManager, textLanguage, diagramId, generatorName)
{
}

QString TrikFSharpMasterGenerator::targetPath()
{
	return QString("%1/%2.fs").arg(mProjectDir, mProjectName);
}

bool TrikFSharpMasterGenerator::supportsGotoGeneration() const
{
	return false;
}

void TrikFSharpMasterGenerator::outputCode(QString const &path, QString const &code)
{
	utils::OutFile out(path);
	QString copy = code;
	out() << copy.replace('\t', "    ").replace("robot.MotorE1", "robot.ServoE1")
			.replace("robot.MotorE2", "robot.ServoE2");
}
