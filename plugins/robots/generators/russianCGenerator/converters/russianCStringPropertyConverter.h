#pragma once

#include <qrutils/expressionsParser/textExpressionProcessorBase.h>
#include <generatorBase/converters/stringPropertyConverter.h>
#include <generatorBase/parts/variables.h>
#include <generatorBase/parts/subprograms.h>

namespace russianC {
namespace converters {

/// Splits given string selecting just numbers and returns converted output port values
class RussianCStringPropertyConverter : public generatorBase::converters::StringPropertyConverter
		, public utils::TextExpressionProcessorBase
{
public:
	explicit RussianCStringPropertyConverter(generatorBase::parts::Variables const &variables);

	QString convert(QString const &data) const override;

protected:
	bool variableExists(QString const &variable) const override;
	QString value(QString const &variable, int index) const override;

private:
	generatorBase::parts::Variables const &mVariables;
};

}
}
