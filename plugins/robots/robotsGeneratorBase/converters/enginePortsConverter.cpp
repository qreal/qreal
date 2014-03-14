#include "enginePortsConverter.h"

using namespace qReal::robots::generators::converters;

EnginePortsConverter::EnginePortsConverter(simple::Binding::ConverterInterface const *oneEngineConverter)
	: RegexpMultiConverter("[^ABC]", oneEngineConverter)
{
}
