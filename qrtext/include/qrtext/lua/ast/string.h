#pragma once

#include "qrtext/core/ast/expression.h"

namespace qrtext {
namespace lua {
namespace ast {

class QRTEXT_EXPORT String : public core::ast::Expression {
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
