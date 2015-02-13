#include "backgroundColorConverter.h"

using namespace trik::converters;

BackgroundColorConverter::BackgroundColorConverter(const QString &pathToTemplates)
	: generatorBase::converters::DynamicPathConverter(pathToTemplates, "backgroundColors/@@DATA@@.t")
{
}
