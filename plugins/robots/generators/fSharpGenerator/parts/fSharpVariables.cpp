#include "fSharpVariables.h"

using namespace fSharp::parts;

FSharpVariables::FSharpVariables(QString const &pathToTemplates
		, interpreterBase::robotModel::RobotModelInterface const &robotModel)
	: Variables(pathToTemplates, robotModel)
{
}

QMap<QString, int> FSharpVariables::intConstants() const
{
	return QMap<QString, int>();
}

QMap<QString, float> FSharpVariables::floatConstants() const
{
	return QMap<QString, float>();
}
