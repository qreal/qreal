#pragma once

#include "codeConverterBase.h"

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

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
}
}
