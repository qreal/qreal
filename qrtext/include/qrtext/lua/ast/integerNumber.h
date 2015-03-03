#pragma once

#include "qrtext/lua/ast/number.h"

namespace qrtext {
namespace lua {
namespace ast {

/// Represents integer number.
class QRTEXT_EXPORT IntegerNumber : public Number
{
public:
	IntegerNumber(const QString &stringRepresentation)
		: Number(stringRepresentation)
	{
	}

private:
	void accept(core::AstVisitorInterface &visitor) const override
	{
		static_cast<LuaAstVisitorInterface *>(&visitor)->visit(*this);
	}
};

}
}
}
