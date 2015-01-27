#pragma once

#include <ostream>

#include <qrkernel/ids.h>

#include "qrtext/declSpec.h"

namespace qrtext {
namespace core {

/// A point in code. Characterized by absolute offset from the beginning of a code fragment (in symbols), line and
/// column. Optionally can contain id of a node and name of a property from where this code originates.
/// Note that coordinates are in symbols, not in bytes.
class QRTEXT_EXPORT Connection
{
public:
	/// Constructor.
	/// @param absolutePosition - position in symbols from the beginning of a text fragment being analyzed.
	/// @param line - line number.
	/// @param column - column number.
	/// @param id - id of a block where the code is located.
	/// @param propertyName - name of a property in a block which uses this code.
	Connection(int absolutePosition, int line, int column, qReal::Id const &id = qReal::Id()
			, const QString &propertyName = "");

	/// Constructor for invalid connection object.
	Connection();

	/// Returns absolute position in symbols from the beginning of a text fragment being parsed.
	int absolutePosition() const;

	/// Returns line number.
	int line() const;

	/// Returns column number.
	int column() const;

	/// Returns id of a block where where the code is located, or empty id if connection is not related to a block.
	qReal::Id const &id() const;

	/// Returns name of a property in a block which uses this code, or empty string, if connection is not related to
	/// some block.
	const QString &propertyName() const;

private:
	int mAbsolutePosition = -1;
	int mLine = -1;
	int mColumn = -1;
	qReal::Id mId;
	QString mPropertyName;
};

inline bool operator ==(Connection const &a, Connection const &b)
{
	return a.absolutePosition() == b.absolutePosition() && a.line() == b.line() && a.column() == b.column();
}

inline bool operator <(Connection const &a, Connection const &b)
{
	return a.absolutePosition() < b.absolutePosition();
}

/// Output operator for gtest.
inline ::std::ostream& operator<<(::std::ostream& os, Connection const &connection)
{
	return os << "(" << connection.absolutePosition() <<", " << connection.line() << ", " << connection.column() << ")";
}

}
}
