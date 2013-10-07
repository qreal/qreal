#include "textExpressionProcessor.h"

#include <QtCore/QRegExp>

using namespace qReal::interpreters::robots::details;

QString TextExpressionProcessor::processExpression(QString const &expression
		, QMap<QString, utils::Number> const &variables) const
{
	// Matching expressions like $<identifier> (escaped with backslash before '$')
	QRegExp const variableRegexp("\\$[A-Za-z_]+\\w*");

	QString result = expression;
	int pos = 0;
	while ((pos = variableRegexp.indexIn(result, pos)) != -1) {
		int toSkip = variableRegexp.matchedLength();
		QString const matchedDeclaration = variableRegexp.cap().remove(0, 1);

		if (pos > 0 && result[pos - 1] == '\\') {
			result.remove(pos - 1, 1);
			--toSkip;
		} else {
			if (variables.contains(matchedDeclaration)) {
				QString const value = variables[matchedDeclaration].toString();
				result.replace(pos, variableRegexp.matchedLength(), value);
				toSkip = value.count();
			}

		}

		pos += toSkip;
	}

	return result;
}
