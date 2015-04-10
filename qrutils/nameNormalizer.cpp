/* Copyright 2007-2015 QReal Research Group
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

#include "nameNormalizer.h"

#include <QtCore/QStringList>
#include <QtCore/QMap>

using namespace utils;

QString NameNormalizer::normalize(const QString &name, bool isUpperFirst)
{
	QString result = name;
	result = result.simplified().replace(" ", "_");
	result = result.replace("::", "_");
	result = isUpperFirst ? upperFirst(result) : lowerFirst(result);
	while (result.endsWith("_")) {
		result.chop(1);
	}
	return result;
}

QString NameNormalizer::normalizeStrongly(const QString &name, bool upperFirst)
{
	QString filteredName;
	foreach (const QChar &character, name) {
		if ((filteredName.isEmpty() && (character.isLetter() || character == '_')) ||
				(!filteredName.isEmpty() && (character.isLetterOrNumber()
						|| character == ' ' || character == '_')))
		{
			filteredName += character;
		}
	}

	if (!name.isEmpty() && filteredName.isEmpty()) {
		return normalizeStrongly("_" + name, upperFirst);
	}

	return russianTranslit(normalize(filteredName, upperFirst));
}

QString NameNormalizer::upperFirst(const QString &string)
{
	if (string.isEmpty()) {
		return "";
	}
	const QStringList tokens = string.split(" ");
	QStringList upperedTokens;
	foreach (const QString &token, tokens) {
		upperedTokens.append(token.at(0).toUpper() + token.mid(1));
	}
	return upperedTokens.join("_");
}

QString NameNormalizer::lowerFirst(const QString &string)
{
	if (string.isEmpty()) {
		return "";
	}
	const QStringList tokens = string.split(" ");
	QStringList loweredTokens;
	foreach (const QString &token, tokens) {
		loweredTokens.append(token.at(0).toLower() + token.mid(1));
	}
	return loweredTokens.join("_");
}

QString NameNormalizer::russianTranslit(const QString &russianString)
{
	const QString lowerRussianLetters[] = { "а", "б", "в", "г", "д", "е", "ё", "ж", "з"
			, "и", "й", "к", "л", "м", "н", "о", "п", "р", "с", "т", "у", "ф"
			, "х", "ц", "ч", "ш", "щ", "ъ", "ы", "ь", "э", "ю", "я" };
	const QString lowerTranslitions[] = { "a", "b", "v", "g", "d", "e", "yo", "zh", "z"
			, "i", "y", "k", "l", "m", "n", "o", "p", "r", "s", "t", "u", "f"
			, "h", "c", "ch", "sh", "sch", "", "y", "", "e", "yu", "ya" };

	QStringList russianLetters, translitions;
	for (uint i = 0; i < sizeof(lowerRussianLetters) / sizeof(*lowerRussianLetters); ++i) {
		russianLetters << lowerRussianLetters[i];
		russianLetters << lowerRussianLetters[i].toUpper();
		translitions << lowerTranslitions[i];
		translitions << lowerTranslitions[i].toUpper();
	}

	QString result;
	foreach (const QChar &letter, russianString) {
		result += russianLetters.contains(letter)
				? translitions[russianLetters.indexOf(letter)] : letter;
	}

	return result;
}
