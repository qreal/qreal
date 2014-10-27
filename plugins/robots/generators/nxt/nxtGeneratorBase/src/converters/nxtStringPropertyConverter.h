#pragma once

#include <generatorBase/converters/stringPropertyConverter.h>
#include <generatorBase/parts/variables.h>
#include <generatorBase/parts/subprograms.h>
#include <qrutils/expressionsParser/textExpressionProcessorBase.h>

namespace nxt {
namespace converters {

/// Splits given string selecting just numbers and returns converted output port values
class NxtStringPropertyConverter : public generatorBase::converters::StringPropertyConverter
		, public utils::TextExpressionProcessorBase
		, public generatorBase::TemplateParametrizedEntity
{
public:
	NxtStringPropertyConverter(QString const &pathToTemplates
			, generatorBase::parts::Variables const &variables
			, generatorBase::parts::Subprograms &subprograms
			, ConverterInterface const &systemVariableNameConverter);

	~NxtStringPropertyConverter() override;

	QString convert(QString const &data) const override;

protected:
	bool variableExists(QString const &variable) const override;
	QString value(QString const &variable, int index) const override;

private:
	generatorBase::parts::Variables const &mVariables;
	ConverterInterface const *mSystemVariableNameConverter;  // Takes ownership
};

}
}
