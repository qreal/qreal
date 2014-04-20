#pragma once

#include "codeConverterBase.h"

namespace generatorBase {
namespace converters {

/// Processes input string treating it as an expression of the float type.
class FloatPropertyConverter : public CodeConverterBase
{
public:
	FloatPropertyConverter(QString const &pathToTemplates
			, simple::Binding::ConverterInterface const *inputPortConverter
			, simple::Binding::ConverterInterface const *outputPortConverter
			, simple::Binding::ConverterInterface const *functionInvocationsConverter);
};

}
}
