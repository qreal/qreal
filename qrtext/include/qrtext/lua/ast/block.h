#pragma once

#include "qrtext/core/ast/node.h"

namespace qrtext {
namespace lua {
namespace ast {

class QRTEXT_EXPORT Block : public core::ast::Node {
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
