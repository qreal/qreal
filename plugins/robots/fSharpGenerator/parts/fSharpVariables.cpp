#include "fSharpVariables.h"

using namespace qReal::robots::generators::parts;

FSharpVariables::FSharpVariables(QString const &pathToTemplates)
	: Variables(pathToTemplates)
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
