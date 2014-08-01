
#include "functionInvocationConverter.h"

using namespace generatorBase::converters;
using namespace qReal;

FunctionInvocationConverter::FunctionInvocationConverter(QString const &pathToTemplates)
	: DynamicPathConverter(pathToTemplates, "functions/@@DATA@@.t")
{
}
