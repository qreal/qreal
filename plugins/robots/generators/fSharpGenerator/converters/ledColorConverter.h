#pragma once

#include <generatorBase/converters/dynamicPathConverter.h>

namespace fSharp {
namespace converters {

/// Converts LED color enum values to method names of Led class in TRIK runtime.
class LedColorConverter : public generatorBase::converters::DynamicPathConverter
{
public:
	explicit LedColorConverter(QString const &pathToTemplates);
};

}
}
