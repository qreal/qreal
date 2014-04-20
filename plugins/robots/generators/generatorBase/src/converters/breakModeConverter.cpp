#include "breakModeConverter.h"

using namespace generatorBase::converters;
using namespace qReal;

BreakModeConverter::BreakModeConverter(QString const &pathToTemplates)
	: EnumConverterBase(pathToTemplates)
{
	addMapping("", "engines/breakMode/break.t");
	addMapping(QString::fromUtf8("тормозить"), "engines/breakMode/break.t");
	addMapping(QString::fromUtf8("скользить"), "engines/breakMode/float.t");
}
