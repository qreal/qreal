#pragma once

#include "enumConverterBase.h"

namespace generatorBase {
namespace converters {

/// Converts wait block comparison property into generator-denendent inequality
/// infix operator.
class InequalitySignConverter : public EnumConverterBase
{
public:
	explicit InequalitySignConverter(QString const &pathToTemplates);
};

}
}
