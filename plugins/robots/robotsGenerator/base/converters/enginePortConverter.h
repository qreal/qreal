#pragma once

#include "enumConverterBase.h"

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

class EnginePortConverter : public EnumConverterBase
{
public:
	explicit EnginePortConverter(QString const &pathToTemplates);
};

}
}
}
}
