/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
	const QSharedPointer<Node> &condition() const
	{
		return mCondition;
	}

	/// Returns true branch node.
	const QSharedPointer<Node> &trueBranch() const
	{
		return mTrueBranch;
	}

	/// Returns false branch node.
	const QSharedPointer<Node> &falseBranch() const
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
