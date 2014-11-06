#pragma once

#include "qrtext/lua/ast/binaryOperator.h"

namespace qrtext {
namespace lua {
namespace ast {

/// Represents integer division operator (//).
class QRTEXT_EXPORT IntegerDivision : public BinaryOperator
{
private:
	void accept(core::AstVisitorInterface &visitor) const override
	{
		static_cast<LuaAstVisitorInterface *>(&visitor)->visit(*this);
	}
};

}
}
}
