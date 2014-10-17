#pragma once

#include "qrtext/lua/ast/unaryOperator.h"

namespace qrtext {
namespace lua {
namespace ast {

/// Represents string or table length operator (#).
class QRTEXT_EXPORT Length : public UnaryOperator
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
