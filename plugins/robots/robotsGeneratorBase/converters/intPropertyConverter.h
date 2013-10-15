#pragma once

#include "codeConverterBase.h"
#include "../parts/variables.h"

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

/// Processes input string treating it as an expression of the int type.
/// Casts this expression to int type if type inferer hints returns other one.
/// Casting code must be specified in "types/cast.t" template.
class IntPropertyConverter : public CodeConverterBase
{
public:
	IntPropertyConverter(QString const &pathToTemplates
			, simple::Binding::ConverterInterface const *inputPortConverter
			, simple::Binding::ConverterInterface const *outputPortConverter
			, simple::Binding::ConverterInterface const *functionInvocationsConverter
			, simple::Binding::ConverterInterface const *typeConverter
			, parts::Variables const *variables);

	virtual ~IntPropertyConverter();

	virtual QString convert(QString const &data) const;

private:
	simple::Binding::ConverterInterface const *mTypeConverter;  // Takes ownership
	parts::Variables const *mVariables;  // Does not take ownership
};

}
}
}
}
