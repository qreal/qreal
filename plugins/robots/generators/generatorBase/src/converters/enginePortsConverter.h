#pragma once

#include "generatorBase/converters/regexpMultiConverter.h"

namespace generatorBase {
namespace converters {

/// Splits given string selecting just letter, digit symbols and underscore and returns converted output port values.
class EnginePortsConverter : public RegexpMultiConverter
{
public:
	EnginePortsConverter(simple::Binding::ConverterInterface const *oneEngineConverter);
};

}
}
