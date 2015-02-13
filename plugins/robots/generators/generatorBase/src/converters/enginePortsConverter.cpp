#include "enginePortsConverter.h"

using namespace generatorBase::converters;
using namespace qReal;

EnginePortsConverter::EnginePortsConverter(const simple::Binding::ConverterInterface *oneEngineConverter)
	: RegexpMultiConverter("\\W+", oneEngineConverter)
{
}
