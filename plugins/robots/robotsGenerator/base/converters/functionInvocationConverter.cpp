#include "functionInvocationConverter.h"

using namespace qReal::robots::generators::converters;

FunctionInvocationConverter::FunctionInvocationConverter(QString const &pathToTemplates)
	: EnumConverterBase(pathToTemplates)
{
	addMapping("random", "functions/random.t");
	addMapping("randomize", "functions/randomize.t");
}
