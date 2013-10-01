#include "trikVariables.h"

using namespace qReal::robots::generators::parts;

QMap<QString, int> TrikVariables::intConstants() const
{
	return QMap<QString, int>();
}

QMap<QString, float> TrikVariables::floatConstants() const
{
	return QMap<QString, float>();
}

QString TrikVariables::intConstantDeclaration() const
{
	return "const var %1 = %2;\n";
}

QString TrikVariables::floatConstantDeclaration() const
{
	return "const var %1 = %2;\n";
}

QString TrikVariables::intVariableDeclaration() const
{
	return "var %1;\n";
}

QString TrikVariables::floatVariableDeclaration() const
{
	return "var %1;\n";
}
