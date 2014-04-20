#include "functionInvocationConverter.h"

using namespace generatorBase::converters;
using namespace qReal;

FunctionInvocationConverter::FunctionInvocationConverter(QString const &pathToTemplates)
	: EnumConverterBase(pathToTemplates)
{
	addMapping("random", "functions/random.t");
	addMapping("randomize", "functions/randomize.t");
}
