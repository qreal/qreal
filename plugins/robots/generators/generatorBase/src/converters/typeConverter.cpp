#include "typeConverter.h"

using namespace generatorBase::converters;
using namespace qReal;

TypeConverter::TypeConverter(QString const &pathToTemplates)
	: EnumConverterBase(pathToTemplates)
{
	addMapping("int", "types/int.t");
	addMapping("float", "types/float.t");
	// TODO: add arrays and so on
}
