#include "inputPortConverter.h"

using namespace qReal::robots::generators::converters;

InputPortConverter::InputPortConverter(QString const &pathToTemplates)
	: EnumConverterBase(pathToTemplates)
{
	addMapping("1", "sensors/port1.t");
	addMapping("2", "sensors/port2.t");
	addMapping("3", "sensors/port3.t");
	addMapping("4", "sensors/port4.t");
}
