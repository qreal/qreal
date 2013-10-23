#include "stringUtils.h"

#include <QtCore/QStringList>

using namespace utils;

QString StringUtils::addIndent(QString const &code, int indent)
{
	if (!indent) {
		return code;
	}

	QStringList const lines = code.split("\n", QString::SkipEmptyParts);
	QString const indentString(indent, '\t');
	QStringList result;
	foreach (QString const &line, lines) {
		result << indentString + line;
	}

	return result.join('\n');
}
