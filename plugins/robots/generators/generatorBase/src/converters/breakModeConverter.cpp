#include "breakModeConverter.h"

using namespace generatorBase::converters;
using namespace qReal;

BreakModeConverter::BreakModeConverter(const QString &pathToTemplates)
	: EnumConverterBase(pathToTemplates)
{
	addMapping("", "engines/breakMode/break.t");
	addMapping("break", "engines/breakMode/break.t");
	addMapping("float", "engines/breakMode/float.t");
}
