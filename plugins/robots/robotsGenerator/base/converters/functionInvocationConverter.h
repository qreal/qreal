#pragma once

#include "enumConverterBase.h"

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

class FunctionInvocationConverter : public EnumConverterBase
{
public:
	/// Possible function identifiers: "random", "randomize"
	explicit FunctionInvocationConverter(QString const &pathToTemplates);
};

}
}
}
}

