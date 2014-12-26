#pragma once

#include "qrtext/lua/ast/number.h"

namespace qrtext {
namespace lua {
namespace ast {

/// Represents floating point number.
class QRTEXT_EXPORT FloatNumber : public Number
{
public:
	FloatNumber(QString const &stringRepresentation)
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
