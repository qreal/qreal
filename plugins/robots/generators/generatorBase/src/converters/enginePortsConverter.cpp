#include "enginePortsConverter.h"

using namespace generatorBase::converters;
using namespace qReal;

EnginePortsConverter::EnginePortsConverter(simple::Binding::ConverterInterface const *oneEngineConverter)
	: RegexpMultiConverter("[^ABC]", oneEngineConverter)
{
}
