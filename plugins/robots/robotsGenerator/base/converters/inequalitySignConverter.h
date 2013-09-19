#pragma once

#include "enumConverterBase.h"

namespace qReal {
namespace robots {
namespace generators {
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
}
}
