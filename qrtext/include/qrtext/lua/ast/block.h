#pragma once

#include "qrtext/lua/ast/node.h"

namespace qrtext {
namespace lua {
namespace ast {

/// Block (or a sequence) of statements.
class QRTEXT_EXPORT Block : public Node
{
public:
	/// Constructor.
	/// @param statements - a list of statements in a block.
	explicit Block(QList<QSharedPointer<Node>> const &statements)
		: mStatements(statements)
	{
	}

	QList<QSharedPointer<Node>> children() const override
	{
		return mStatements;
	}

private:
	void accept(core::AstVisitorInterface &visitor) const override
	{
		static_cast<LuaAstVisitorInterface *>(&visitor)->visit(*this);
	}

	QList<QSharedPointer<Node>> mStatements;
};

}
}
}
