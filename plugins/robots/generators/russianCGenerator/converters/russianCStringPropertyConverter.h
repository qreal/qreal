#pragma once

#include <qrutils/expressionsParser/textExpressionProcessorBase.h>
#include <generatorBase/converters/stringPropertyConverter.h>
#include <generatorBase/parts/variables.h>
#include <generatorBase/parts/subprograms.h>

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

	QString convert(QString const &data) const override;

protected:
	bool variableExists(QString const &variable) const override;
	QString value(QString const &variable) const override;

private:
	parts::Variables const &mVariables;
};

}
}
}
}
