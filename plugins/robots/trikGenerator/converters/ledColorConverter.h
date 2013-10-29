#pragma once

#include "base/converters/enumConverterBase.h"

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

/// Converts LED color enum values to method names of Led class in TRIK runtime.
class LedColorConverter : public EnumConverterBase
{
public:
	LedColorConverter(QString const &pathToTemplates);
};

}
}
}
}
