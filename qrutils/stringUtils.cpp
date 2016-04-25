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

QString StringUtils::escape(const QString &string)
{
	auto transformedString = string;
	for (int i = 0; i < transformedString.length(); ++i) {
		const auto replace = [&](const QString &str) {
			transformedString.replace(i, 1, str);
			++i;
		};

		switch (transformedString[i].toLatin1()) {
		case '\\':
			replace("\\\\");
			break;
		case '\'':
			replace("\\'");
			break;
		case '"':
			replace("\\\"");
			break;
		case '\?':
			replace("\\?");
			break;
		case '\a':
			replace("\\a");
			break;
		case '\b':
			replace("\\b");
			break;
		case '\f':
			replace("\\f");
			break;
		case '\n':
			replace("\\n");
			break;
		case '\r':
			replace("\\r");
			break;
		case '\t':
			replace("\\t");
			break;
		case '\v':
			replace("\\v");
			break;
		default:
			break;
		}
	}

	return transformedString;
}

QString StringUtils::unescape(const QString &string)
{
	auto transformedString = string;
	for (int i = 0; i < transformedString.length() - 1; ++i) {
		if (transformedString[i] == '\\') {
			const auto replace = [&](QChar c) {
				transformedString.replace(i, 2, c);
			};

			switch (transformedString[i + 1].toLatin1()) {
			case '\\':
				replace('\\');
				break;
			case '\'':
				replace('\'');
				break;
			case '"':
				replace('"');
				break;
			case '?':
				replace('\?');
				break;
			case 'a':
				replace('\a');
				break;
			case 'b':
				replace('\b');
				break;
			case 'f':
				replace('\f');
				break;
			case 'n':
				replace('\n');
				break;
			case 'r':
				replace('\r');
				break;
			case 't':
				replace('\t');
				break;
			case 'v':
				replace('\v');
				break;
			default:
				break;
			}
		}
	}

	return transformedString;
}
