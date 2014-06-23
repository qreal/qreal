#pragma once

#include <generatorBase/converters/stringPropertyConverter.h>
#include <generatorBase/parts/variables.h>
#include <generatorBase/parts/subprograms.h>
#include <qrutils/expressionsParser/textExpressionProcessorBase.h>

namespace nxtOsek {
namespace converters {

/// Splits given string selecting just numbers and returns converted output port values
class NxtStringPropertyConverter : public generatorBase::converters::StringPropertyConverter
		, public utils::TextExpressionProcessorBase
{
public:
	NxtStringPropertyConverter(generatorBase::parts::Variables const &variables
			, generatorBase::parts::Subprograms &subprograms);

	QString convert(QString const &data) const override;

protected:
	bool variableExists(QString const &variable) const override;
	QString value(QString const &variable) const override;

private:
	generatorBase::parts::Variables const &mVariables;
};

}
}
