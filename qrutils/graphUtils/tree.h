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

#include <QtCore/QList>

#include "qrutils/utilsDeclSpec.h"

namespace qReal
{

/// A base class for tree structures
class QRUTILS_EXPORT Tree
{
public:
	Tree();
	virtual ~Tree() {}

	QListIterator<Tree *> children() const;

	void appendChild(Tree * const child);
	void insertChild(int index, Tree * const child);
	void removeChild(Tree * const child);

	/// Visits all tree nodes in parent-leftChild-rightChild order
	/// with @param visitor functor invocation in each node
	/// with this node as parameter
	template<typename Visitor>
	void parentLeftRight(Visitor &visitor)
	{
		visitor(this);
		foreach (Tree *node, mChildren) {
			if (node) {
				node->parentLeftRight(visitor);
			}
		}
	}

	/// Visits all tree nodes in leftChild-rightChild-parent order
	/// with @param visitor functor invocation in each node
	/// with this node as parameter
	template<typename Visitor>
	void leftRightParent(Visitor &visitor)
	{
		foreach (Tree *node, mChildren) {
			node->parentLeftRight(visitor);
		}
		visitor(this);
	}

	/// Visits all tree nodes in rightChild-leftChild-parent order
	/// with @param visitor functor invocation in each node
	/// with this node as parameter
	template<typename Visitor>
	void rightLeftParent(Visitor &visitor)
	{
		QListIterator<Tree *> childNodes = children();
		childNodes.toBack();
		while (childNodes.hasPrevious()) {
			childNodes.previous()->rightLeftParent(visitor);
		}
		visitor(this);
	}

private:
	QList<Tree *> mChildren;
};

}
