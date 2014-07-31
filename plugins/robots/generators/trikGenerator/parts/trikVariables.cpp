#include "trikVariables.h"

using namespace trik::parts;

TrikVariables::TrikVariables(QString const &pathToTemplates
		, interpreterBase::robotModel::RobotModelInterface const &robotModel)
	: Variables(pathToTemplates, robotModel)
{
}

QMap<QString, int> TrikVariables::intConstants() const
{
	return QMap<QString, int>();
}

QMap<QString, float> TrikVariables::floatConstants() const
{
	return QMap<QString, float>();
}

QStringList TrikVariables::expressions(qrRepo::RepoApi const &api) const
{
	QStringList lineDetectorExpressions;
	for (qReal::Id const &block : api.elementsByType("TrikLineDetectorToVariable")) {
		if (api.hasProperty(block, "Variable")) {
			lineDetectorExpressions << api.stringProperty(block, "Variable") + " = 0";
		}
	}

	return Variables::expressions(api) + lineDetectorExpressions;
}
