#pragma once

#include "enumConverterBase.h"

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

/// Converts an input port with given number into generator-dependent port representation.
/// Resulting code must be specified in "sensors/port*.t" templates.
class InputPortConverter : public EnumConverterBase
{
public:
	explicit InputPortConverter(QString const &pathToTemplates);
};

}
}
}
}
