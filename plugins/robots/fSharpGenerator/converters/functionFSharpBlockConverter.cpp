#include "functionFSharpBlockConverter.h"
#include <QRegExp>

using namespace qReal::robots::generators::converters;

FunctionFSharpBlockConverter::FunctionFSharpBlockConverter(QString const &pathToTemplates
		, simple::Binding::ConverterInterface const *inputPortConverter
		, simple::Binding::ConverterInterface const *outputPortConverter
		, simple::Binding::ConverterInterface const *functionInvocationsConverter)
	: CodeConverterBase(pathToTemplates
			, inputPortConverter
			, outputPortConverter
			, functionInvocationsConverter)
{
}

QString FunctionFSharpBlockConverter::convert(QString const &data) const
{
	QString const preparedCode = CodeConverterBase::convert(data);

	QStringList const lines = preparedCode.split(';', QString::SkipEmptyParts);
	QStringList result;

	foreach (QString const &line, lines) {
		QRegExp regExp("<=|>=|==");

		if(!line.trimmed().contains(regExp)) {
			result.append(line.trimmed().replace("=", "<-"));
		}
		else {
			result.append(line.trimmed());
		}
	}

	return result.join('\n');
}
