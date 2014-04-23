#pragma once

#include <generatorBase/converters/enumConverterBase.h>

namespace trik {
namespace converters {

/// Converts display background color enum values to Qt color names.
class BackgroundColorConverter : public generatorBase::converters::EnumConverterBase
{
public:
	explicit BackgroundColorConverter(QString const &pathToTemplates);
};

}
}
