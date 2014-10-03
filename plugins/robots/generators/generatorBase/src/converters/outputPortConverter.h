#pragma once

#include "generatorBase/converters/enumConverterBase.h"

namespace generatorBase {
namespace converters {

/// Converts an output port with given id into generator-dependent port representation.
/// Resulting code must be specified in "engines/engine*.t" templates.
class OutputPortConverter : public EnumConverterBase
{
public:
	explicit OutputPortConverter(QString const &pathToTemplates);
};

}
}
