#include "colorConverter.h"

using namespace nxt::converters;

ColorConverter::ColorConverter(QString const &pathToTemplates)
	: generatorBase::converters::DynamicPathConverter(pathToTemplates, "colors/@@DATA@@.t")
{
}
