#include "colorConverter.h"

using namespace generatorBase::converters;
using namespace qReal;

ColorConverter::ColorConverter(QString const &pathToTemplates)
	: DynamicPathConverter(pathToTemplates, "colors/@@DATA@@.t")
{
}
