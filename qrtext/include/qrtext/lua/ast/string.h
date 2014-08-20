#pragma once

#include "qrtext/lua/ast/expression.h"

namespace qrtext {
namespace lua {
namespace ast {

class QRTEXT_EXPORT String : public Expression {
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
}
