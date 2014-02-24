#include "textExpressionProcessorBase.h"

#include <QtCore/QRegExp>

using namespace utils;

QString TextExpressionProcessorBase::processExpression(QString const &expression) const
{
	QStringList metVariables;
	return processExpression(expression, metVariables);
}

QString TextExpressionProcessorBase::processExpression(QString const &expression
		, QStringList &metVariables) const
{
	// Matching expressions like $<identifier> (escaped with backslash before '$')
	QRegExp const variableRegexp("\\$[A-Za-z_]+\\w*");

	QString result = expression;
	int pos = 0;
	while ((pos = variableRegexp.indexIn(result, pos)) != -1) {
		int toSkip = variableRegexp.matchedLength();
		QString const matchedDeclaration = variableRegexp.cap().remove(0, 1).toLower();

		if (pos > 0 && result[pos - 1] == '\\') {
			result.remove(pos - 1, 1);
			--toSkip;
		} else {
			if (variableExists(matchedDeclaration)) {
				metVariables << matchedDeclaration;
				QString const variableValue = value(matchedDeclaration);
				result.replace(pos, variableRegexp.matchedLength(), variableValue);
				toSkip = variableValue.count();
			}

		}

		pos += toSkip;
	}

	return result;
}
