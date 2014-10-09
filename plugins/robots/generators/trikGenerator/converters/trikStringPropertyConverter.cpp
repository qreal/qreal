#include "trikStringPropertyConverter.h"

#include <qrtext/core/types/any.h>

using namespace trik;
using namespace converters;
using namespace generatorBase;
using namespace parts;

TrikStringPropertyConverter::TrikStringPropertyConverter(Variables const &variables
		, ConverterInterface const &systemVariableNameConverter)
	: mVariables(variables)
	, mSystemVariableNameConverter(&systemVariableNameConverter)
{
}

TrikStringPropertyConverter::~TrikStringPropertyConverter()
{
	delete mSystemVariableNameConverter;
}

QString TrikStringPropertyConverter::convert(QString const &data) const
{
	QString const preparedString = StringPropertyConverter::convert(data);
	QStringList metVariables;
	QString const formatString = TextExpressionProcessorBase::processExpression(preparedString, metVariables);
	QStringList rolledVariables;
	for (QString const &variable : metVariables) {
		/// @todo: variable name may not exactly match system variable but have it as substring.
		rolledVariables << mSystemVariableNameConverter->convert(variable);
	}

	QString const formatVariables = rolledVariables.join(", ");
	return rolledVariables.isEmpty()
			? formatString
			: QString("%1.format(%2)").arg(formatString, formatVariables);
}

bool TrikStringPropertyConverter::variableExists(QString const &variable) const
{
	return !mVariables.expressionType(variable)->is<qrtext::core::types::Any>();
}

QString TrikStringPropertyConverter::value(QString const &variable, int index) const
{
	Q_UNUSED(variable)
	return QString("{%1}").arg(index);
}
