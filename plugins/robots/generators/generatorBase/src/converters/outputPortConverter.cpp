#include "outputPortConverter.h"

using namespace generatorBase::converters;
using namespace qReal;

OutputPortConverter::OutputPortConverter(QString const &pathToTemplates)
	: EnumConverterBase(pathToTemplates)
{
	addMapping("A", "engines/engineA.t");
	addMapping("B", "engines/engineB.t");
	addMapping("C", "engines/engineC.t");
}
