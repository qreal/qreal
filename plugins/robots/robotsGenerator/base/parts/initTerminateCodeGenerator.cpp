#include "initTerminateCodeGenerator.h"

using namespace qReal::robots::generators::parts;

InitTerminateCodeGenerator::InitTerminateCodeGenerator(QString const &pathToTemplates)
	: TemplateParametrizedEntity(pathToTemplates)
{
}

InitTerminateCodeGenerator::~InitTerminateCodeGenerator()
{
}

void InitTerminateCodeGenerator::reinit()
{
}

QString InitTerminateCodeGenerator::initCode()
{
	return QString();
}

QString InitTerminateCodeGenerator::terminateCode()
{
	return QString();
}

QString InitTerminateCodeGenerator::isrHooksCode()
{
	return QString();
}
