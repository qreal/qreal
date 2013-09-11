#pragma once

#include "codeConverterBase.h"

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

class FunctionBlockConverter : public CodeConverterBase
{
public:
	FunctionBlockConverter(QString const &pathToTemplates
		, simple::Binding::ConverterInterface const *inputPortConverter
		, simple::Binding::ConverterInterface const *outputPortConverter
		, simple::Binding::ConverterInterface const *functionInvocationsConverter);

	virtual QString convert(QString const &data) const;
};

}
}
}
}
