#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

/// Class that represents "if" node.
class IfNode : public Node
{
public:
	/// Constructor.
	/// @param condition - condition of if statement.
	/// @param trueBranch - called if condition is true.
	/// @param falseBranch - called otherwise.
	IfNode(const QSharedPointer<Node> &condition
			, const QSharedPointer<Node> &trueBranch
			, const QSharedPointer<Node> &falseBranch = QSharedPointer<ast::Node>())
		: mCondition(condition)
		, mTrueBranch(trueBranch)
		, mFalseBranch(falseBranch)
	{
	}

	/// Returns condition node.
	QSharedPointer<Node> condition() const
	{
		return mCondition;
	}

	/// Returns true branch node.
	QSharedPointer<Node> trueBranch() const
	{
		return mTrueBranch;
	}

	/// Returns false branch node.
	QSharedPointer<Node> falseBranch() const
	{
		return mFalseBranch;
	}

	QList<QSharedPointer<Node>> children() const override
	{
		return {mCondition, mTrueBranch, mFalseBranch};
	}

private:
	QSharedPointer<Node> mCondition;
	QSharedPointer<Node> mTrueBranch;
	QSharedPointer<Node> mFalseBranch;
};
}
}
