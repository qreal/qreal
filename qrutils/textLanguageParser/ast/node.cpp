#include "textLanguageParser/ast/node.h"

using namespace textLanguageParser::ast;

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

void Node::connect(details::Token const &token)
{
	mRanges << token.range();
	qSort(mRanges.begin(), mRanges.end(), [](Range a, Range b) { return a.start() < b.start(); });
}

void Node::connect(QSharedPointer<Node> const &node)
{
	if (node) {
		mRanges << node->ranges();
		qSort(mRanges.begin(), mRanges.end(), [](Range a, Range b) { return a.start() < b.start(); });
	}
}
