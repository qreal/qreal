/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
	Connection(int absolutePosition, int line, int column, const qReal::Id &id = qReal::Id()
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
	const qReal::Id &id() const;

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

inline bool operator ==(const Connection &a, const Connection &b)
{
	return a.absolutePosition() == b.absolutePosition() && a.line() == b.line() && a.column() == b.column();
}

inline bool operator <(const Connection &a, const Connection &b)
{
	return a.absolutePosition() < b.absolutePosition();
}

/// Output operator for gtest.
inline ::std::ostream& operator<<(::std::ostream& os, const Connection &connection)
{
	return os << "(" << connection.absolutePosition() <<", " << connection.line() << ", " << connection.column() << ")";
}

}
}
