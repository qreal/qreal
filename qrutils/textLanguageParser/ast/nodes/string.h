#pragma once

#include "textLanguageParser/ast/nodes/expression.h"

namespace textLanguageParser {
namespace ast {

class QRUTILS_EXPORT String : public Expression {
public:
	String(QString const &string)
		: mString(string)
	{
	}

	QString const &string() const {
		return mString;
	}

private:
	QString mString;
};

}
}
