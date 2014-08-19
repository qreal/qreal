#pragma once

#include "qrtext/core/ast/expression.h"

namespace qrtext {
namespace lua {
namespace ast {

class QRTEXT_EXPORT Number : public core::ast::Expression {
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
