#include "nameNormalizer.h"

#include <QtCore/QStringList>
#include <QtCore/QMap>

using namespace utils;

QString NameNormalizer::normalize(QString const &name, bool const isUpperFirst)
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

QString NameNormalizer::normalizeStrongly(QString const &name, bool const upperFirst)
{
	QString filteredName;
	foreach (QChar const &character, name) {
		if ((filteredName.isEmpty() && (character.isLetter() || character == '_')) ||
				(!filteredName.isEmpty() && (character.isLetterOrNumber()
						|| character == ' ' || character == '_')))
		{
			filteredName += character;
		}
	}

	return russianTranslit(normalize(filteredName, upperFirst));
}

QString NameNormalizer::upperFirst(QString const &string)
{
	if (string.isEmpty()) {
		return "";
	}
	QStringList const tokens = string.split(" ");
	QStringList upperedTokens;
	foreach (QString const &token, tokens) {
		upperedTokens.append(token.at(0).toUpper() + token.mid(1));
	}
	return upperedTokens.join("_");
}

QString NameNormalizer::lowerFirst(QString const &string)
{
	if (string.isEmpty()) {
		return "";
	}
	QStringList const tokens = string.split(" ");
	QStringList loweredTokens;
	foreach (QString const &token, tokens) {
		loweredTokens.append(token.at(0).toLower() + token.mid(1));
	}
	return loweredTokens.join("_");
}

QString NameNormalizer::russianTranslit(QString const &russianString)
{
	QString const lowerRussianLetters[] = { "а", "б", "в", "г", "д", "е", "ё", "ж", "з"
			, "и", "й", "к", "л", "м", "н", "о", "п", "р", "с", "т", "у", "ф"
			, "х", "ц", "ч", "ш", "щ", "ъ", "ы", "ь", "э", "ю", "я" };
	QString const lowerTranslitions[] = { "a", "b", "v", "g", "d", "e", "yo", "zh", "z"
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
	foreach (QChar const &letter, russianString) {
		result += russianLetters.contains(letter)
				? translitions[russianLetters.indexOf(letter)] : letter;
	}

	return result;
}
