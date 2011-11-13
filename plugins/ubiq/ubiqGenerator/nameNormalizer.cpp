#include "nameNormalizer.h"

#include <QtCore/QStringList>

using namespace ubiq::generator;

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

QString NameNormalizer::upperFirst(QString const &string)
{
	if (string.size() < 1) {
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
	if (string.size() < 1) {
		return "";
	}
	QStringList const tokens = string.split(" ");
	QStringList loweredTokens;
	foreach (QString const &token, tokens) {
		loweredTokens.append(token.at(0).toLower() + token.mid(1));
	}
	return loweredTokens.join("_");
}
