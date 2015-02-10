#pragma once

#include <QtCore/QString>

#include "qrutils/utilsDeclSpec.h"

namespace utils {

/// Contains useful methods extending QString class
class QRUTILS_EXPORT StringUtils
{
public:
	/// Splits string into a set of lines and prepends each line with
	/// the given @arg indentString repeated @arg indent times.
	static QString addIndent(const QString &code, int indent, const QString &indentString/* = "\t"*/);

	/// Wraps the given string into double quotes and returns the result.
	static QString wrap(const QString &string);
};

}
