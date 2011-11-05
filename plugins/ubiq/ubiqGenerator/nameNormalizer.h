#pragma once

#include <QString>

class NameNormalizer
{
public:
	static QString normalize(QString const &name, bool const upperFirst = true);

private:
	static QString upperFirst(QString const &string);
	static QString lowerFirst(QString const &string);
};
