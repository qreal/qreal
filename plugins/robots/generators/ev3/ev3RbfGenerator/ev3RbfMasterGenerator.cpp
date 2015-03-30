#include "ev3RbfMasterGenerator.h"

using namespace ev3::rbf;

Ev3RbfMasterGenerator::Ev3RbfMasterGenerator(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, const interpreterBase::robotModel::RobotModelManagerInterface &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage
		, const qReal::Id &diagramId
		, const QString &generatorName)
	: Ev3MasterGeneratorBase(repo, errorReporter, robotModelManager, textLanguage, diagramId, generatorName)
{
}
