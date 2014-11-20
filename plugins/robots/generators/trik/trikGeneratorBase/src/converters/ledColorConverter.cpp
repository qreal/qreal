#include "ledColorConverter.h"

using namespace trik::converters;

LedColorConverter::LedColorConverter(QString const &pathToTemplates)
	: DynamicPathConverter(pathToTemplates, "ledColors/@@DATA@@.t")
{
}
