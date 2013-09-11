#pragma once

#include "enumConverterBase.h"

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

class BreakModeConverter : public EnumConverterBase
{
public:
	explicit BreakModeConverter(QString const &pathToTemplates);
};

}
}
}
}
