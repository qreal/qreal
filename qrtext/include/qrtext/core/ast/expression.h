#pragma once

#include "qrtext/core/ast/node.h"

namespace qrtext {
namespace core {
namespace ast {

/// Base class for all expressions.
class Expression : public Node
{
private:
	void accept(AstVisitorInterface &visitor) const override
	{
		visitor.visit(*this);
	}
};

}
}
}
