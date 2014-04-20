#include "russianCStringPropertyConverter.h"

using namespace russianC;
using namespace converters;

RussianCStringPropertyConverter::RussianCStringPropertyConverter(generatorBase::parts::Variables const &variables)
	: mVariables(variables)
{
}

QString RussianCStringPropertyConverter::convert(QString const &data) const
{
	QString const preparedString = StringPropertyConverter::convert(data);
	QStringList metVariables;
	QString const formatString = TextExpressionProcessorBase::processExpression(preparedString, metVariables);

	// Nxt OSEK does not support floating point numbers printing, so hello hacks
	// (we print each float variable like two int ones separated with '.')
	QStringList hackedVariables;
	foreach (QString const &variable, metVariables) {
		if (mVariables.expressionType(variable) == generatorBase::enums::variableType::intType) {
			hackedVariables << variable;
		} else {
			hackedVariables << "(целое)" + variable
					<< QString("((целое)((%1 - (целое))%1) * 1000))").arg(variable);
		}
	}

	QString const formatVariables = hackedVariables.join(", ");
	return hackedVariables.isEmpty()
			? formatString
			: QString("форматировать(%1, %2)").arg(formatString, formatVariables);
}

bool RussianCStringPropertyConverter::variableExists(QString const &variable) const
{
	return mVariables.expressionType(variable) != generatorBase::enums::variableType::unknown;
}

QString RussianCStringPropertyConverter::value(QString const &variable) const
{
	return mVariables.expressionType(variable) == generatorBase::enums::variableType::intType ? "%ц" : "%ц.%ц";
}
