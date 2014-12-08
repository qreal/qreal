#pragma once

#include <QtCore/QString>

#include "utilsDeclSpec.h"

namespace utils {

/// Contains useful methods extending QString class
class QRUTILS_EXPORT StringUtils
{
public:
	/// Splits string into a set of lines and prepends each line with
	/// the given @arg indentString repeated @arg indent times.
	static QString addIndent(QString const &code, int indent, QString const &indentString/* = "\t"*/);
};

}
