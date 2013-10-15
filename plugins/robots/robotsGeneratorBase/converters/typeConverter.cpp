#include "typeConverter.h"

using namespace qReal::robots::generators::converters;

TypeConverter::TypeConverter(QString const &pathToTemplates)
	: EnumConverterBase(pathToTemplates)
{
	addMapping("int", "types/int.t");
	addMapping("float", "types/float.t");
	// TODO: add arrays and so on
}
