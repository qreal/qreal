#pragma once

#include <generatorBase/converters/dynamicPathConverter.h>

namespace trik {
namespace converters {

/// Converts display background color enum values to Qt color names.
class BackgroundColorConverter : public generatorBase::converters::DynamicPathConverter
{
public:
	explicit BackgroundColorConverter(const QString &pathToTemplates);
};

}
}
