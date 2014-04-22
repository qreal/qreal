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
