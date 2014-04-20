#pragma once

#include "generatorBase/simpleGenerators/binding.h"

namespace generatorBase {
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
