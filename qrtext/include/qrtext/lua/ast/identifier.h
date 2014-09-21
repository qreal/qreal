#pragma once

#include "qrtext/lua/ast/expression.h"

namespace qrtext {
namespace lua {
namespace ast {

class QRTEXT_EXPORT Identifier : public Expression
{
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
