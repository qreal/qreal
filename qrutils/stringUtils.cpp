/* Copyright 2007-2015 QReal Research Group, Dmitry Mordvinov
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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

QString StringUtils::capitalizeFirstLetter(const QString &string)
{
	return string.at(0).toUpper() + string.mid(1);
}

QString StringUtils::lowercaseFirstLetter(const QString &string)
{
	return string.at(0).toLower() + string.mid(1);
}

QString StringUtils::dequote(const QString &string)
{
	return (string.startsWith("'") && string.endsWith("'")) || (string.startsWith("\"") && string.endsWith("\""))
		? string.mid(1, string.length() - 2)
		: string;
}
