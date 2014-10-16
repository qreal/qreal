#include "nxtStringPropertyConverter.h"

#include <qrtext/core/types/any.h>
#include <qrtext/lua/types/integer.h>
#include <qrtext/lua/types/float.h>

using namespace nxtOsek;
using namespace converters;
using namespace generatorBase;
using namespace parts;

NxtStringPropertyConverter::NxtStringPropertyConverter(Variables const &variables
		, parts::Subprograms &subprograms
		, ConverterInterface const &systemVariableNameConverter)
	: mVariables(variables)
	, mSystemVariableNameConverter(&systemVariableNameConverter)
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

NxtStringPropertyConverter::~NxtStringPropertyConverter()
{
	delete mSystemVariableNameConverter;
}

QString NxtStringPropertyConverter::convert(QString const &data) const
{
	QString const preparedString = StringPropertyConverter::convert(data);
	QStringList metVariables;
	QString const formatString = TextExpressionProcessorBase::processExpression(preparedString, metVariables);

	// Nxt OSEK does not support floating point numbers printing, so hello hacks
	// (we print each float variable like two int ones separated with '.')
	QStringList hackedVariables;
	for (QString const &variable : metVariables) {
		/// @todo: variable name may not exactly match system variable but have it as substring.
		QString const rolledExpression = mSystemVariableNameConverter->convert(variable);
		if (mVariables.expressionType(variable)->is<qrtext::lua::types::Float>()) {
			hackedVariables << "(int)" + rolledExpression
					<< QString("((int)((%1 - (int)%1) * 1000))").arg(rolledExpression);
		} else {
			hackedVariables << rolledExpression;
		}
	}

	QString const formatVariables = hackedVariables.join(", ");
	return hackedVariables.isEmpty()
			? formatString
			: QString("formatString(%1, %2)").arg(formatString, formatVariables);
}

bool NxtStringPropertyConverter::variableExists(QString const &variable) const
{
	return !mVariables.expressionType(variable)->is<qrtext::core::types::Any>();
}

QString NxtStringPropertyConverter::value(QString const &variable, int index) const
{
	Q_UNUSED(index)
	return mVariables.expressionType(variable)->is<qrtext::lua::types::Integer>() ? "%d"
			: mVariables.expressionType(variable)->is<qrtext::lua::types::Float>() ? "%d.%d"
			: "%<unsupported variable type, will be implemented later!>";
}
