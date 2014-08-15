#pragma once

#include "qrutils/textLanguageParser/ast/nodes/expression.h"

namespace textLanguageParser {
namespace ast {

class QRUTILS_EXPORT String : public Expression {
public:
	String(QString const &string)
		: mString(string)
	{
		// Chop away quotes.
		mString.chop(1);
		mString.remove(0, 1);
	}

	QString const &string() const {
		return mString;
	}

private:
	QString mString;
};

}
}
