#include "breakModeConverter.h"

using namespace qReal::robots::generators::converters;

BreakModeConverter::BreakModeConverter(QString const &pathToTemplates)
	: EnumConverterBase(pathToTemplates)
{
	addMapping("", "engines/breakMode/break.t");
	addMapping("break", "engines/breakMode/break.t");
	addMapping("float", "engines/breakMode/float.t");
}
