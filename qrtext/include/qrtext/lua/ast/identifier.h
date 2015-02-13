#pragma once

#include "qrtext/lua/ast/expression.h"

namespace qrtext {
namespace lua {
namespace ast {

class QRTEXT_EXPORT Identifier : public Expression
{
public:
	Identifier(const QString &name)
		: mName(name)
	{
	}

	const QString &name() const {
		return mName;
	}

private:
	void accept(core::AstVisitorInterface &visitor) const override
	{
		static_cast<LuaAstVisitorInterface *>(&visitor)->visit(*this);
	}

	QString mName;
};

}
}
}
