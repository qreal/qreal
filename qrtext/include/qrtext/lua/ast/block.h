#pragma once

#include "qrtext/lua/ast/node.h"

namespace qrtext {
namespace lua {
namespace ast {

/// Block (or a sequence) of statements.
class QRTEXT_EXPORT Block : public Node {
public:
	/// Constructor.
	/// @param statements - a list of statements in a block.
	Block(QList<QSharedPointer<Node>> const &statements)
		: mStatements(statements)
	{
	}

	QList<QSharedPointer<Node>> children() const override
	{
		return mStatements;
	}

private:
	QList<QSharedPointer<Node>> mStatements;
};

}
}
}
