#pragma once

#include "qrutils/utilsDeclSpec.h"

namespace textLanguageParser {
namespace ast {

class QRUTILS_EXPORT Connection {
public:
	Connection(int absolutePosition, int line, int column);
	Connection();

	int absolutePosition() const;
	int line() const;
	int column() const;

private:
	int mAbsolutePosition = -1;
	int mLine = -1;
	int mColumn = -1;
};

}
}
