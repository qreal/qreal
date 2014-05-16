#include "typeConverter.h"

using namespace generatorBase::converters;
using namespace qReal;

TypeConverter::TypeConverter(QString const &pathToTemplates)
	: DynamicPathConverter(pathToTemplates, "types/@@DATA@@.t")
{
}
