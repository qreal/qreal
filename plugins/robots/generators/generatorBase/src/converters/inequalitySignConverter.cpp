#include "inequalitySignConverter.h"

using namespace generatorBase::converters;
using namespace qReal;

InequalitySignConverter::InequalitySignConverter(QString const &pathToTemplates)
	: DynamicPathConverter(pathToTemplates, "signs/@@DATA@@.t")
{
}
