#pragma once

#include "codeConverterBase.h"

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

class BoolPropertyConverter : public CodeConverterBase
{
public:
	BoolPropertyConverter(QString const &pathToTemplates
			, simple::Binding::ConverterInterface const *inputPortConverter
			, simple::Binding::ConverterInterface const *outputPortConverter
			, simple::Binding::ConverterInterface const *functionInvocationsConverter
			, bool needInverting);

	virtual QString convert(QString const &data) const;

private:
	QString invert(QString const &expression) const;

	bool mNeedInverting;
};

}
}
}
}
