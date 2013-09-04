#pragma once

#include "enumConverterBase.h"

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

class InputPortConverter : public EnumConverterBase
{
public:
	explicit InputPortConverter(QString const &pathToTemplates);
};

}
}
}
}
