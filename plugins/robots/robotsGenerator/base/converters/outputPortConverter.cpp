#include "outputPortConverter.h"

using namespace qReal::robots::generators::converters;

OutputPortConverter::OutputPortConverter(QString const &pathToTemplates)
	: EnumConverterBase(pathToTemplates)
{
	addMapping("A", "engines/engineA.t");
	addMapping("B", "engines/engineB.t");
	addMapping("C", "engines/engineC.t");
}
