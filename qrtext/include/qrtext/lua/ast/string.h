#pragma once

#include "qrtext/lua/ast/expression.h"

namespace qrtext {
namespace lua {
namespace ast {

/// Represents Lua string literal.
class QRTEXT_EXPORT String : public Expression
{
public:
	/// Constructor.
	/// @param string - value of a literal (without quotes).
	String(QString const &string)
		: mString(string)
	{
	}

	/// Returns value of a string (without quotes).
	QString const &string() const {
		return mString;
	}

private:
	void accept(core::AstVisitorInterface &visitor) const override
	{
		static_cast<LuaAstVisitorInterface *>(&visitor)->visit(*this);
	}

	QString mString;
};

}
}
}
