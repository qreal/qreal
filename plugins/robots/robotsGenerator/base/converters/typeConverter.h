#pragma once

#include "enumConverterBase.h"

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

class TypeConverter : public EnumConverterBase
{
public:
	explicit TypeConverter(QString const &pathToTemplates);
};

}
}
}
}
