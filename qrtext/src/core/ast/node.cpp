#include "qrtext/core/ast/node.h"

using namespace qrtext::core;
using namespace qrtext::core::ast;

Connection const Node::noConnection = Connection();

Connection const &Node::start() const
{
	if (mRanges.isEmpty()) {
		return noConnection;
	} else {
		return mRanges.first().start();
	}
}

Connection const &Node::end() const
{
	if (mRanges.isEmpty()) {
		return noConnection;
	} else {
		return mRanges.last().end();
	}
}

QList<Range> const &Node::ranges() const
{
	return mRanges;
}

void Node::connect(QSharedPointer<Node> const &node)
{
	if (node) {
		connect(node->ranges());
	}
}

void Node::connect(QList<Range> const &ranges)
{
	mRanges << ranges;
	qSort(mRanges.begin(), mRanges.end(), [](Range a, Range b) { return a.start() < b.start(); });
}

void Node::acceptRecursively(AstVisitorInterface &visitor) const
{
	for (auto const &node : children()) {
		node->acceptRecursively(visitor);
	}

	accept(visitor);
}

void Node::accept(AstVisitorInterface &visitor) const
{
	visitor.visit(*this);
}
