#pragma once

#include "enumConverterBase.h"

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

/// Converts given function invocation into useful generator-dependent code.
/// Possible function identifiers: "random", "randomize".
class FunctionInvocationConverter : public EnumConverterBase
{
public:
	explicit FunctionInvocationConverter(QString const &pathToTemplates);
};

}
}
}
}

