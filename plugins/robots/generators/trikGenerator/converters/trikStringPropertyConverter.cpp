#include "trikStringPropertyConverter.h"

using namespace trik;
using namespace converters;
using namespace generatorBase;
using namespace parts;

TrikStringPropertyConverter::TrikStringPropertyConverter(Variables const &variables)
	: mVariables(variables)
{
}

QString TrikStringPropertyConverter::convert(QString const &data) const
{
	QString const preparedString = StringPropertyConverter::convert(data);
	QStringList metVariables;
	QString const formatString = TextExpressionProcessorBase::processExpression(preparedString, metVariables);

	QString const formatVariables = metVariables.join(", ");
	return metVariables.isEmpty()
			? formatString
			: QString("%1.format(%2)").arg(formatString, formatVariables);
}

bool TrikStringPropertyConverter::variableExists(QString const &variable) const
{
	return mVariables.expressionType(variable) != enums::variableType::unknown;
}

QString TrikStringPropertyConverter::value(QString const &variable, int index) const
{
	Q_UNUSED(variable)
	return QString("{%1}").arg(index);
}
