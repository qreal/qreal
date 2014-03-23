#pragma once

#include "simpleGenerators/binding.h"

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

/// Transforms input string into a valid cpp-style indentifier skipping all
/// inappropriate symbols.
class NameNormalizerConverter : public simple::Binding::ConverterInterface
{
public:
	QString convert(QString const &data) const override;
};

}
}
}
}
