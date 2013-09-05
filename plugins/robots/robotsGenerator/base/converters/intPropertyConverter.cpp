#include "intPropertyConverter.h"

using namespace qReal::robots::generators::converters;

IntPropertyConverter::IntPropertyConverter(QString const &pathToTemplates
		, simple::Binding::ConverterInterface const *inputPortConverter
		, simple::Binding::ConverterInterface const *outputPortConverter
		, simple::Binding::ConverterInterface const *functionInvocationsConverter
		, simple::Binding::ConverterInterface const *typeConverter
		, parts::Variables const *variables)
	: CodeConverterBase(pathToTemplates
			, inputPortConverter
			, outputPortConverter
			, functionInvocationsConverter)
	, mTypeConverter(typeConverter)
	, mVariables(variables)
{
}

IntPropertyConverter::~IntPropertyConverter()
{
	delete mTypeConverter;
}

QString IntPropertyConverter::convert(QString const &data) const
{
	QString const preparedCode = CodeConverterBase::convert(data);
	if (preparedCode.isEmpty()) {
		return "0";
	}

	enums::variableType::VariableType const expressionType = mVariables->expressionType(preparedCode);
	if (expressionType == enums::variableType::intType) {
		return preparedCode;
	}

	QString castTemplate = readTemplate("types/cast.t");
	QString const intType = mTypeConverter->convert("int");
	castTemplate.replace("@@TYPE@@", intType);
	castTemplate.replace("@@EXPRESSION@@", preparedCode);
	return castTemplate;
}

