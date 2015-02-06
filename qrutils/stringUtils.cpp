#include "stringUtils.h"

#include <QtCore/QStringList>

using namespace utils;

QString StringUtils::addIndent(const QString &code, int indent, const QString &oneIndentString)
{
	if (!indent || oneIndentString.isEmpty()) {
		return code;
	}

	const QStringList lines = code.split("\n", QString::SkipEmptyParts);
	const QString indentString = oneIndentString.repeated(indent);
	QStringList result;
	for (const QString &line : lines) {
		result << indentString + line;
	}

	return result.join('\n');
}

QString StringUtils::wrap(const QString &string)
{
	return "\"" + string + "\"";
}
