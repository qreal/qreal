#include "generatorBase/parts/initTerminateCodeGenerator.h"

using namespace generatorBase::parts;

InitTerminateCodeGenerator::InitTerminateCodeGenerator(const QString &pathToTemplates)
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
