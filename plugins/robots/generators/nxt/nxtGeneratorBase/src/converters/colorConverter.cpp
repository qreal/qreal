#include "colorConverter.h"

using namespace nxt::converters;

ColorConverter::ColorConverter(const QString &pathToTemplates)
	: generatorBase::converters::DynamicPathConverter(pathToTemplates, "colors/@@DATA@@.t")
{
}
