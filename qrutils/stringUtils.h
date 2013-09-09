#pragma once

#include <QtCore/QString>

namespace utils {

class StringUtils
{
public:
	static QString addIndent(QString const &code, int indent);
};

}
