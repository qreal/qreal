#include "enginePortConverter.h"

using namespace qReal::robots::generators::converters;

EnginePortConverter::EnginePortConverter(QString const &pathToTemplates)
	: EnumConverterBase(pathToTemplates)
{
	addMapping("A", "engines/engineA.t");
	addMapping("B", "engines/engineB.t");
	addMapping("C", "engines/engineC.t");
}
