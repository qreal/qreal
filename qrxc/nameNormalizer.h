#pragma once

#include <QtCore/QString>

class NameNormalizer
{
public:
	static QString normalize(const QString &name);

private:
	static QString upperFirst(const QString &string);
};
