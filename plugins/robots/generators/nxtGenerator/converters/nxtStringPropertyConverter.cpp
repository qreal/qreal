#include "nxtStringPropertyConverter.h"

using namespace nxtOsek;
using namespace converters;
using namespace generatorBase;
using namespace parts;

NxtStringPropertyConverter::NxtStringPropertyConverter(Variables const &variables
		, parts::Subprograms &subprograms)
	: mVariables(variables)
{
	QString const formatStringCode =
			"char *formatString(char *format, ...)\n"\
			"{\n"\
			"	char buffer[256];\n"\
			"	va_list args;\n"\
			"	va_start(args, format);\n"\
			"	vsnprintf(buffer, 256, format, args);\n"\
			"	va_end(args);\n"\
			"	return buffer;\n"\
			"}";
	subprograms.appendManualSubprogram("formatString", formatStringCode);
}

QString NxtStringPropertyConverter::convert(QString const &data) const
{
	QString const preparedString = StringPropertyConverter::convert(data);
	QStringList metVariables;
	QString const formatString = TextExpressionProcessorBase::processExpression(preparedString, metVariables);

	// Nxt OSEK does not support floating point numbers printing, so hello hacks
	// (we print each float variable like two int ones separated with '.')
	QStringList hackedVariables;
	foreach (QString const &variable, metVariables) {
		if (mVariables.expressionType(variable) == enums::variableType::intType) {
			hackedVariables << variable;
		} else {
			hackedVariables << "(int)" + variable
					<< QString("((int)((%1 - (int)%1) * 1000))").arg(variable);
		}
	}

	QString const formatVariables = hackedVariables.join(", ");
	return hackedVariables.isEmpty()
			? formatString
			: QString("formatString(%1, %2)").arg(formatString, formatVariables);
}

bool NxtStringPropertyConverter::variableExists(QString const &variable) const
{
	return mVariables.expressionType(variable) != enums::variableType::unknown;
}

QString NxtStringPropertyConverter::value(QString const &variable) const
{
	return mVariables.expressionType(variable) == enums::variableType::intType ? "%d" : "%d.%d";
}
