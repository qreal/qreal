#include "ev3RbfMasterGenerator.h"

using namespace ev3::rbf;

Ev3RbfMasterGenerator::Ev3RbfMasterGenerator(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage
		, qReal::Id const &diagramId
		, QString const &generatorName)
	: Ev3MasterGeneratorBase(repo, errorReporter, robotModelManager, textLanguage, diagramId, generatorName)
{
}

QString Ev3RbfMasterGenerator::targetPath()
{
	return QString("%1/%2.rbf").arg(mProjectDir, mProjectName);
}

bool Ev3RbfMasterGenerator::supportsGotoGeneration() const
{
	return false;
}
