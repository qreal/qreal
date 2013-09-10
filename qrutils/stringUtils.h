#pragma once

#include <QtCore/QString>

#include "utilsDeclSpec.h"

namespace utils {

class QRUTILS_EXPORT StringUtils
{
public:
	static QString addIndent(QString const &code, int indent);
};

}
