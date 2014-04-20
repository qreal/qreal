#pragma once

#include "enumConverterBase.h"

namespace generatorBase {
namespace converters {

/// Processes input string treating it as repository property of the 'BreakMode'
/// type. Mapped values must be specified in "engines/breakMode/break.t" and
/// "engines/breakMode/float.t" templates.
class BreakModeConverter : public EnumConverterBase
{
public:
	explicit BreakModeConverter(QString const &pathToTemplates);
};

}
}
