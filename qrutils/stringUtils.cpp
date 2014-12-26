#include "stringUtils.h"

#include <QtCore/QStringList>

using namespace utils;

QString StringUtils::addIndent(QString const &code, int indent, QString const &oneIndentString)
{
	if (!indent || oneIndentString.isEmpty()) {
		return code;
	}

	QStringList const lines = code.split("\n", QString::SkipEmptyParts);
	QString const indentString = oneIndentString.repeated(indent);
	QStringList result;
	for (QString const &line : lines) {
		result << indentString + line;
	}

	return result.join('\n');
}
