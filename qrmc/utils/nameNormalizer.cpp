#include "nameNormalizer.h"

#include <QtCore/QStringList>
#include <QtCore/QDebug>

QString NameNormalizer::normalize(QString const &name)
{
	QString result = name;
	if (name.contains("::"))
		result = result.right(result.length() - result.lastIndexOf("::") - 2);
	result = result.simplified().replace(" ", "_");
	result = result.replace("::", "_");
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
