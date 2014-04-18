#pragma once

#include <generatorBase/converters/stringPropertyConverter.h>
#include <generatorBase/parts/variables.h>
#include <generatorBase/parts/subprograms.h>
#include <qrutils/expressionsParser/textExpressionProcessorBase.h>

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

/// Splits given string selecting just numbers and returns converted output port values
class NxtStringPropertyConverter : public StringPropertyConverter
		, public utils::TextExpressionProcessorBase
{
public:
	NxtStringPropertyConverter(parts::Variables const &variables
			, parts::Subprograms &subprograms);

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
