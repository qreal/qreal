#pragma once

#include "qrtext/core/ast/node.h"

namespace qrtext {
namespace core {

/// A temporary pair of nodes, result of concatenation parser. Shall be removed from resulting tree.
class TemporaryPair : public ast::Node {
public:
	/// Constructor. Takes left and right subtrees.
	TemporaryPair(QSharedPointer<ast::Node> const &left, QSharedPointer<ast::Node> const &right)
		: mLeft(left), mRight(right)
	{
	}

	/// Returns left subtree.
	QSharedPointer<ast::Node> const &left() const
	{
		return mLeft;
	}

	/// Returns right subtree.
	QSharedPointer<ast::Node> const &right() const
	{
		return mRight;
	}

private:
	QSharedPointer<ast::Node> mLeft;
	QSharedPointer<ast::Node> mRight;
};

}
}
