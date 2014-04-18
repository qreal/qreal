#include "colorConverter.h"

using namespace qReal::robots::generators::converters;

ColorConverter::ColorConverter(QString const &pathToTemplates)
	: EnumConverterBase(pathToTemplates)
{
	addMapping(QString::fromUtf8("Красный"), "colors/red.t");
	addMapping(QString::fromUtf8("Зелёный"), "colors/green.t");
	addMapping(QString::fromUtf8("Синий"), "colors/blue.t");
	addMapping(QString::fromUtf8("Чёрный"), "colors/black.t");
	addMapping(QString::fromUtf8("Жёлтый"), "colors/yellow.t");
	addMapping(QString::fromUtf8("Белый"), "colors/white.t");
}
