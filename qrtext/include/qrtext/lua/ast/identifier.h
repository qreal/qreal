#pragma once

#include "qrtext/core/ast/expression.h"

namespace qrtext {
namespace lua {
namespace ast {

class QRTEXT_EXPORT Identifier : public core::ast::Expression {
public:
	Identifier(QString const &name)
		: mName(name)
	{
	}

	QString const &name() const {
		return mName;
	}

private:
	QString mName;
};

}
}
}
