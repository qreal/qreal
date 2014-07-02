#include "fSharpStringPropertyConverter.h"

using namespace fSharp;
using namespace converters;
using namespace generatorBase;
using namespace parts;

FSharpStringPropertyConverter::FSharpStringPropertyConverter(Variables const &variables)
	: mVariables(variables)
{
}

QString FSharpStringPropertyConverter::convert(QString const &data) const
{
	QString const preparedString = StringPropertyConverter::convert(data);
	QStringList metVariables;
	QString const formatString = TextExpressionProcessorBase::processExpression(preparedString, metVariables);

	QString const formatVariables = metVariables.join(", ");
	return metVariables.isEmpty()
			? formatString
			: QString("%1.format(%2)").arg(formatString, formatVariables);
}

bool FSharpStringPropertyConverter::variableExists(QString const &variable) const
{
	return mVariables.expressionType(variable) != enums::variableType::unknown;
}

QString FSharpStringPropertyConverter::value(QString const &variable, int index) const
{
	Q_UNUSED(variable)
	return QString("{%1}").arg(index);
}
