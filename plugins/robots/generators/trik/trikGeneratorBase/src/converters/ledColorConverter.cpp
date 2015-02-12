#include "ledColorConverter.h"

using namespace trik::converters;

LedColorConverter::LedColorConverter(const QString &pathToTemplates)
	: DynamicPathConverter(pathToTemplates, "ledColors/@@DATA@@.t")
{
}
