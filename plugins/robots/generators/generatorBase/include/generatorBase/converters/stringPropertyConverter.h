#pragma once

#include "generatorBase/simpleGenerators/binding.h"
#include "generatorBase/robotsGeneratorDeclSpec.h"

namespace generatorBase {
namespace converters {

/// Replaces all inappropriate symbols in input string for valid usage in
/// code as string literal (for example replaces quotes into escaped ones)
class ROBOTS_GENERATOR_EXPORT StringPropertyConverter : public simple::Binding::ConverterInterface
{
public:
	QString convert(QString const &data) const override;
};

}
}
