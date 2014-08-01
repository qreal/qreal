#include "backgroundColorConverter.h"

using namespace fSharp::converters;

BackgroundColorConverter::BackgroundColorConverter(QString const &pathToTemplates)
	: generatorBase::converters::DynamicPathConverter(pathToTemplates, "backgroundColors/@@DATA@@.t")
{
}
