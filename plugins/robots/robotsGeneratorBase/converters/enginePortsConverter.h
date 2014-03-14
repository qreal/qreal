#pragma once

#include "regexpMultiConverter.h"

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

/// Splits given string selecting just 'A', 'B' and 'C' symbols and returns
/// converted output port values.
class EnginePortsConverter : public RegexpMultiConverter
{
public:
	explicit EnginePortsConverter(simple::Binding::ConverterInterface const *oneEngineConverter);
};

}
}
}
}
