#pragma once

#include "node.h"

namespace textLanguageParser {
namespace ast {

/// @todo Move it somewhere, it is not a part of AST.
class TemporaryPair : public Node {
public:
	TemporaryPair(QSharedPointer<Node> left, QSharedPointer<Node> right)
		: mLeft(left), mRight(right)
	{
	}

	QSharedPointer<Node> left() const
	{
		return mLeft;
	}

	QSharedPointer<Node> right() const
	{
		return mRight;
	}

private:
	QSharedPointer<Node> mLeft;
	QSharedPointer<Node> mRight;
};

}
}
