#pragma once

#include <converters/stringPropertyConverter.h>
#include <qrutils/expressionsParser/textExpressionProcessorBase.h>
#include <parts/variables.h>
#include <parts/subprograms.h>

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

/// Splits given string selecting just numbers and returns converted output port values
class RussianCStringPropertyConverter : public StringPropertyConverter
		, public utils::TextExpressionProcessorBase
{
public:
	explicit RussianCStringPropertyConverter(parts::Variables const &variables);

	virtual QString convert(QString const &data) const;

protected:
	virtual bool variableExists(QString const &variable) const;
	virtual QString value(QString const &variable) const;

private:
	parts::Variables const &mVariables;
};

}
}
}
}
