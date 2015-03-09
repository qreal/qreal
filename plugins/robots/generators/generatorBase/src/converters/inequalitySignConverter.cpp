#include "inequalitySignConverter.h"

using namespace generatorBase::converters;
using namespace qReal;

InequalitySignConverter::InequalitySignConverter(const QString &pathToTemplates)
	: DynamicPathConverter(pathToTemplates, "signs/@@DATA@@.t")
{
}
