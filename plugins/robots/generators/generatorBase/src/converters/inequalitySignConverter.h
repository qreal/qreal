#pragma once

#include "generatorBase/converters/dynamicPathConverter.h"

namespace generatorBase {
namespace converters {

/// Converts wait block comparison property into generator-dependent inequality infix operator.
class InequalitySignConverter : public DynamicPathConverter
{
public:
	explicit InequalitySignConverter(QString const &pathToTemplates);
};

}
}
