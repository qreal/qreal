#include "trikVariables.h"

using namespace qReal::robots::generators::parts;

TrikVariables::TrikVariables(QString const &pathToTemplates)
	: Variables(pathToTemplates)
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
