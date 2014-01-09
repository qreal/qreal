#pragma once

#include "enumConverterBase.h"

namespace qReal {
namespace robots {
namespace generators {
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
}
}
