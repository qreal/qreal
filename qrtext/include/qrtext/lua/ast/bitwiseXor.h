#pragma once

#include "qrtext/lua/ast/binaryOperator.h"

namespace qrtext {
namespace lua {
namespace ast {

/// Represents bitwise xor operator (~).
class QRTEXT_EXPORT BitwiseXor : public BinaryOperator
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
