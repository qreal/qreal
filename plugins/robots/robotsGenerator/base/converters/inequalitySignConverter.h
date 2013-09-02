#pragma once

#include "enumConverterBase.h"

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

class InequalitySignConverter : public EnumConverterBase
{
public:
	explicit InequalitySignConverter(QString const &pathToTemplates);
};

}
}
}
}
