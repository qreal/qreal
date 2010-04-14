#pragma once

#include <QString>

class NameNormalizer
{
public:
	static QString normalize(QString const &name);

private:
	static QString upperFirst(QString const &string);
};
