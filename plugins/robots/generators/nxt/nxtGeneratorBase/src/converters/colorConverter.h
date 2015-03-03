#pragma once

#include "generatorBase/converters/dynamicPathConverter.h"

namespace nxt {
namespace converters {

/// Converts 'Color' enum in WaitForColor blocks into generator-specific code.
/// Resulting code must be specified in templates from "colors" folder.
class ColorConverter : public generatorBase::converters::DynamicPathConverter
{
public:
	explicit ColorConverter(const QString &pathToTemplates);
};

}
}
