#pragma once

#include <generatorBase/converters/enumConverterBase.h>

namespace trik {
namespace converters {

/// Converts LED color enum values to method names of Led class in TRIK runtime.
class LedColorConverter : public generatorBase::converters::EnumConverterBase
{
public:
	explicit LedColorConverter(QString const &pathToTemplates);
};

}
}
