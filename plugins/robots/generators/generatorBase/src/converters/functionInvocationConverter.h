#pragma once

#include "generatorBase/converters/dynamicPathConverter.h"

namespace generatorBase {
namespace converters {

/// Converts given function invocation into useful generator-dependent code.
/// Possible function identifiers: "random", "randomize".
class FunctionInvocationConverter : public DynamicPathConverter
{
public:
	explicit FunctionInvocationConverter(QString const &pathToTemplates);
};

}
}
