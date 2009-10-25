#include "nameNormalizer.h"

#include <QStringList>

QString NameNormalizer::normalize(QString const &name)
{
	QString result = name;
	result = result.toLower().simplified().replace(" ", "_");
	result = upperFirst(result);
	while (result.endsWith("_"))
	{
		result.chop(1);
	}
	return result;
}

QString NameNormalizer::upperFirst(QString const &string)
{
	if (string.size() < 1)
	{
		return "";
	}
	QStringList tokens = string.split(" ");
	QStringList upperedTokens;
	foreach (QString token, tokens)
	{
		upperedTokens.append(token.at(0).toUpper() + token.mid(1));
	}
	return upperedTokens.join("_");
}
