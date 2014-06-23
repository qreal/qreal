#include "generatorBase/parts/functions.h"

using namespace generatorBase::parts;
using namespace qReal;

Functions::Functions(QString const &pathToTemplates)
	: InitTerminateCodeGenerator(pathToTemplates)
{
}

void Functions::reinit()
{
	mFunctions.clear();
}

QString Functions::initCode()
{
	return mFunctions.join('\n');
}

void Functions::registerFunctionInInitialization(QString const &body)
{
	mFunctions << body;
}
