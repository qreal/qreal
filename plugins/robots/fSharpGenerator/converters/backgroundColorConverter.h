#pragma once

#include <converters/enumConverterBase.h>

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

/// Converts display background color enum values to Qt color names.
class BackgroundColorConverter : public EnumConverterBase
{
public:
	explicit BackgroundColorConverter(QString const &pathToTemplates);
};

}
}
}
}
