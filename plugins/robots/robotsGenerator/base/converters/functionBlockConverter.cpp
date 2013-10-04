#include "functionBlockConverter.h"

using namespace qReal::robots::generators::converters;

FunctionBlockConverter::FunctionBlockConverter(QString const &pathToTemplates
		, simple::Binding::ConverterInterface const *inputPortConverter
		, simple::Binding::ConverterInterface const *outputPortConverter
		, simple::Binding::ConverterInterface const *functionInvocationsConverter)
	: CodeConverterBase(pathToTemplates
			, inputPortConverter
			, outputPortConverter
			, functionInvocationsConverter)
{
}

QString FunctionBlockConverter::convert(QString const &data) const
{
	QString const preparedCode = CodeConverterBase::convert(data);

	QStringList const lines = preparedCode.split(';', QString::SkipEmptyParts);
	QStringList result;
	foreach (QString const &line, lines) {
		result.append(line.trimmed() + ";");
	}

	return result.join('\n');
}
