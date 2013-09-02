#pragma once

#include "enumConverterBase.h"

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

class ColorConverter : public EnumConverterBase
{
public:
	explicit ColorConverter(QString const &pathToTemplates);
};

}
}
}
}
