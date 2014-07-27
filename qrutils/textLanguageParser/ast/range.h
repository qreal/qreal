#pragma once

#include "textLanguageParser/ast/connection.h"

#include "qrutils/utilsDeclSpec.h"

namespace textLanguageParser {
namespace ast {

class QRUTILS_EXPORT Range {
public:
	Range(int startAbsolutePosition, int startLine, int startColumn
			, int endAbsolutePosition, int endLine, int endColumn);

	Range();

	Connection const &start() const;
	Connection const &end() const;

private:
	Connection mStart;
	Connection mEnd;
};

}
}
