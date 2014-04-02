#include "ledColorConverter.h"

using namespace qReal::robots::generators::converters;

LedColorConverter::LedColorConverter(QString const &pathToTemplates)
	: EnumConverterBase(pathToTemplates)
{
	addMapping(QString::fromUtf8("Красный"), "ledColors/red.t");
	addMapping(QString::fromUtf8("Зелёный"), "ledColors/green.t");
	addMapping(QString::fromUtf8("Выключен"), "ledColors/off.t");
}
