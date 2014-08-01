#pragma once

#include <generatorBase/converters/stringPropertyConverter.h>
#include <generatorBase/parts/variables.h>
#include <generatorBase/parts/subprograms.h>
#include <qrutils/expressionsParser/textExpressionProcessorBase.h>

namespace fSharp {
namespace converters {

/// Splits given string selecting just numbers and returns converted output port values
class FSharpStringPropertyConverter : public generatorBase::converters::StringPropertyConverter
		, public utils::TextExpressionProcessorBase
{
public:
	explicit FSharpStringPropertyConverter(generatorBase::parts::Variables const &variables);

	QString convert(QString const &data) const override;

protected:
	bool variableExists(QString const &variable) const override;
	QString value(QString const &variable, int index) const override;

private:
	generatorBase::parts::Variables const &mVariables;
	int mCurrentIndex;
};

}
}
