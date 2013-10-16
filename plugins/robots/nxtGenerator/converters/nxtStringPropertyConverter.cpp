#include "nxtStringPropertyConverter.h"

using namespace qReal::robots::generators;
using namespace converters;
using namespace parts;

NxtStringPropertyConverter::NxtStringPropertyConverter(Variables const &variables
		, parts::Subprograms &subprograms)
	: mVariables(variables)
{
	QString const formatStringCode =
			"char *formatString(char *format, ...)\n"\
			"{\n"\
			"	char *buffer[256]n"\
			"	va_list args;\n"\
			"	va_start(args, format);\n"\
			"	vsnprintf(buffer, 256, format, args);\n"\
			"	va_end(args);\n"\
			"	return beffer;\n"\
			"}";
	subprograms.appendManualSubprogram("formatString", formatStringCode);
}

QString NxtStringPropertyConverter::convert(QString const &data) const
{
	QString const preparedString = StringPropertyConverter::convert(data);
	QStringList metVariables;
	QString const formatString = TextExpressionProcessorBase::processExpression(preparedString, metVariables);
	QString const formatVariables =  metVariables.join(", ");
	return QString("formatString(%1, %2)").arg(formatString, formatVariables);
}

bool NxtStringPropertyConverter::variableExists(QString const &variable) const
{
	return mVariables.expressionType(variable) != enums::variableType::unknown;
}

QString NxtStringPropertyConverter::value(QString const &variable) const
{
	return mVariables.expressionType(variable) == enums::variableType::intType ? "%d" : "%.4f";
}
