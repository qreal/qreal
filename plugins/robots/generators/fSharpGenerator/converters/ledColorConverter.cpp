#include "ledColorConverter.h"

using namespace fSharp::converters;

LedColorConverter::LedColorConverter(QString const &pathToTemplates)
	: DynamicPathConverter(pathToTemplates, "ledColors/@@DATA@@.t")
{
}
