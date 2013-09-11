#include "boolPropertyConverter.h"

using namespace qReal::robots::generators::converters;

BoolPropertyConverter::BoolPropertyConverter(QString const &pathToTemplates
		, simple::Binding::ConverterInterface const *inputPortConverter
		, simple::Binding::ConverterInterface const *outputPortConverter
		, simple::Binding::ConverterInterface const *functionInvocationsConverter
		, bool needInverting)
	: CodeConverterBase(pathToTemplates
			, inputPortConverter
			, outputPortConverter
			, functionInvocationsConverter)
	, mNeedInverting(needInverting)
{
}

QString BoolPropertyConverter::convert(QString const &data) const
{
	QString const preparedCode = CodeConverterBase::convert(data);
	return mNeedInverting ? invert(preparedCode) : preparedCode;
}

QString BoolPropertyConverter::invert(QString const &expression) const
{
	QString invertTemplate = readTemplate("conditional/negation.t");
	invertTemplate.replace("@@CONDITION@@", expression);
	return invertTemplate;
}
