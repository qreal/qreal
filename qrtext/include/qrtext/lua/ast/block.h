#pragma once

#include "qrtext/lua/ast/node.h"

namespace qrtext {
namespace lua {
namespace ast {

class QRTEXT_EXPORT Block : public Node {
public:
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
