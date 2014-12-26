#pragma once

#include "qrtext/lua/ast/expression.h"

namespace qrtext {
namespace lua {
namespace ast {

/// Base class for integer and float numbers.
class QRTEXT_EXPORT Number : public Expression
{
public:
	/// Constructor.
	/// @param stringRepresentation - string representation of a number. Actual value shall be calculated by
	/// interpreter.
	Number(QString const &stringRepresentation)
		: mStringRepresentation(stringRepresentation)
	{
	}

	/// Returns string representation of a number.
	QString const &stringRepresentation() const {
		return mStringRepresentation;
	}

private:
	void accept(core::AstVisitorInterface &visitor) const override
	{
		static_cast<LuaAstVisitorInterface *>(&visitor)->visit(*this);
	}

	QString mStringRepresentation;
};

}
}
}
