#include <utils/textExpressionProcessor.h>

#include <QtCore/QRegExp>

using namespace utils;

TextExpressionProcessor::TextExpressionProcessor(QMap<QString, utils::Number *> const &variables)
	: mVariables(variables)
{
}

QString TextExpressionProcessor::processExpression(QString const &expression) const
{
	return TextExpressionProcessorBase::processExpression(expression);
}

bool TextExpressionProcessor::variableExists(QString const &variable) const
{
	return mVariables.contains(variable);
}

QString TextExpressionProcessor::value(QString const &variable) const
{
	return mVariables[variable]->toString();
}
