#pragma once

#include "qrtext/lua/ast/expression.h"

namespace qrtext {
namespace lua {
namespace ast {

class QRTEXT_EXPORT Number : public Expression {
public:
	Number(QString const &stringRepresentation)
		: mStringRepresentation(stringRepresentation)
	{
	}

	QString const &stringRepresentation() const {
		return mStringRepresentation;
	}

private:
	QString mStringRepresentation;
};

}
}
}
