#include "backgroundColorConverter.h"

using namespace trik::converters;

BackgroundColorConverter::BackgroundColorConverter(QString const &pathToTemplates)
	: generatorBase::converters::DynamicPathConverter(pathToTemplates, "backgroundColors/@@DATA@@.t")
{
}
