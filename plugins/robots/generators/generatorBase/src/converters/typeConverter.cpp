#include "typeConverter.h"

using namespace generatorBase::converters;
using namespace qReal;

TypeConverter::TypeConverter(const QString &pathToTemplates)
	: DynamicPathConverter(pathToTemplates, "types/@@DATA@@.t")
{
}
