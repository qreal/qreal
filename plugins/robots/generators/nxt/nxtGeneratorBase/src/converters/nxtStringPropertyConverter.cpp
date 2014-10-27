#include "nxtStringPropertyConverter.h"

#include <qrtext/core/types/any.h>
#include <qrtext/lua/types/integer.h>
#include <qrtext/lua/types/float.h>

using namespace nxt;
using namespace converters;
using namespace generatorBase;
using namespace parts;

NxtStringPropertyConverter::NxtStringPropertyConverter(QString const &pathToTemplates
		, Variables const &variables
		, parts::Subprograms &subprograms
		, ConverterInterface const &systemVariableNameConverter)
	: TemplateParametrizedEntity(pathToTemplates)
	, mVariables(variables)
	, mSystemVariableNameConverter(&systemVariableNameConverter)
{
	QString const formatStringCode = readTemplate("printTextBlock/formatStringImplementation.t");
	if (!formatStringCode.isEmpty()) {
		subprograms.appendManualSubprogram(readTemplate("printTextBlock/formatStringIdentifier.t"), formatStringCode);
	}
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
	QString const intType = readTemplate("types/int.t");
	QString const castToInt = readTemplate("types/cast.t").replace("@@TYPE@@", intType);
	for (QString const &variable : metVariables) {
		QString const rolledExpression = mSystemVariableNameConverter->convert(variable);
		if (mVariables.expressionType(variable)->is<qrtext::lua::types::Float>()) {
			QString const fractionalPart = QString("@@EXPRESSION@@ - " + castToInt)
					.replace("@@EXPRESSION@@", rolledExpression);
			QString const threeLastDigits = QString("(%1) * 1000").arg(fractionalPart);
			hackedVariables << QString(castToInt).replace("@@EXPRESSION@@", rolledExpression)
				<< QString(castToInt).replace("@@EXPRESSION@@", threeLastDigits);
		} else {
			hackedVariables << rolledExpression;
		}
	}

	QString const formatVariables = hackedVariables.join(", ");
	return hackedVariables.isEmpty()
			? formatString
			: QString("%1(%2, %3)").arg(readTemplate("printTextBlock/formatStringIdentifier.t")
					, formatString, formatVariables);
}

bool NxtStringPropertyConverter::variableExists(QString const &variable) const
{
	return !mVariables.expressionType(variable)->is<qrtext::core::types::Any>();
}

QString NxtStringPropertyConverter::value(QString const &variable, int index) const
{
	Q_UNUSED(index)
	return mVariables.expressionType(variable)->is<qrtext::lua::types::Integer>()
			? readTemplate("printTextBlock/intSpecifier.t")
			: mVariables.expressionType(variable)->is<qrtext::lua::types::Float>()
					? readTemplate("printTextBlock/floatSpecifier.t")
					: readTemplate("printTextBlock/unsupportedMessage.t");
}
