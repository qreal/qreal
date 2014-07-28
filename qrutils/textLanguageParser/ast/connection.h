#pragma once

#include <ostream>

#include "qrutils/utilsDeclSpec.h"

namespace textLanguageParser {
namespace ast {

/// A point in code. Characterized by absolute offset from the beginning of a code fragment (in symbols), line and
/// column. Note that coordinates are in symbols, not in bytes.
class QRUTILS_EXPORT Connection {
public:
	/// Constructor.
	/// @param absolutePosition - position in symbols from the beginning of a text fragment being analyzed.
	/// @param line - line number.
	/// @param column - column number.
	Connection(int absolutePosition, int line, int column);

	/// Constructor for invalid connection object.
	Connection();

	/// Returns absolute position in symbols from the beginning of a text fragment being parsed.
	int absolutePosition() const;

	/// Returns line number.
	int line() const;

	/// Returns column number.
	int column() const;

private:
	int mAbsolutePosition = -1;
	int mLine = -1;
	int mColumn = -1;
};

inline bool operator ==(Connection const &a, Connection const &b)
{
	return a.absolutePosition() == b.absolutePosition() && a.line() == b.line() && a.column() == b.column();
}

}
}

/// Output operator for gtest.
inline ::std::ostream& operator<<(::std::ostream& os, const textLanguageParser::ast::Connection& connection)
{
	return os << "(" << connection.absolutePosition() <<", " << connection.line() << ", " << connection.column() << ")";
}
