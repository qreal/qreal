#pragma once

#include "textLanguageParser/ast/connection.h"

#include "qrutils/utilsDeclSpec.h"

namespace textLanguageParser {
namespace ast {

/// A range between two connections in code. Characterizes location of a token in code.
class QRUTILS_EXPORT Range {
public:
	/// Constructor that takes start and end coordinates of a range.
	Range(int startAbsolutePosition, int startLine, int startColumn
			, int endAbsolutePosition, int endLine, int endColumn);

	/// Constructor of an invalid range object.
	Range();

	/// Returns start coordinate of a range.
	Connection const &start() const;

	/// Returns end coordinate of a range.
	Connection const &end() const;

private:
	Connection mStart;
	Connection mEnd;
};

}
}
