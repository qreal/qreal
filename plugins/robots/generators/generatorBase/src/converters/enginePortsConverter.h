#pragma once

#include "generatorBase/converters/regexpMultiConverter.h"

namespace generatorBase {
namespace converters {

/// Splits given string selecting just 'A', 'B' and 'C' symbols and returns converted output port values.
class EnginePortsConverter : public RegexpMultiConverter
{
public:
	EnginePortsConverter(simple::Binding::ConverterInterface const *oneEngineConverter);
};

}
}
