#pragma once

#include "enumConverterBase.h"

namespace generatorBase {
namespace converters {

/// Converts input string into generate-dependent type identifier. Possible inputs:
/// "int" and "float". Resulting code must be specified in "types" folder.
class TypeConverter : public EnumConverterBase
{
public:
	explicit TypeConverter(QString const &pathToTemplates);
};

}
}
