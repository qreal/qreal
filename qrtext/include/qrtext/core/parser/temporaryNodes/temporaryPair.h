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

#include "qrtext/core/ast/node.h"

namespace qrtext {
namespace core {

/// A temporary pair of nodes, result of concatenation parser. Shall be removed from resulting tree.
class TemporaryPair : public ast::Node
{
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
