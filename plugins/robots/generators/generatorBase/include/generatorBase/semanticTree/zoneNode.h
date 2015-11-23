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

#include "semanticNode.h"

namespace generatorBase {
namespace semantics {

/// Stores a list of semantic nodes. In fact an analogue of block operator in c++
class ZoneNode : public SemanticNode
{
public:
	explicit ZoneNode(QObject *parent = 0);

	/// Returns 'true' if this zone doesn`t have children
	bool isEmpty() const;

	/// Adds given node into child list`s end
	void appendChild(SemanticNode *node);

	/// Adds given nodes list into child list`s end
	void appendChildren(QLinkedList<SemanticNode *> const &nodes);

	/// Adds given node into the child list after the given otherNode node
	void insertAfrer(SemanticNode *after, SemanticNode *node);

	/// Removes given node from children list without deleting the child itself
	void removeChild(SemanticNode *node);

	/// Removes last node from children list without deleting the child itself
	SemanticNode *removeLast();

	/// Removes all the children starting from specified node without deleting
	/// themselves and returns removed tail. Removes all if node is null.
	QLinkedList<SemanticNode *> removeStartingFrom(SemanticNode *node);

	/// Returns parent semantic node. The result is never nullptr.
	SemanticNode *parentNode();

	/// Returns an immediate child folowing after the given node or nullptr if no such node
	/// (i.e. @arg child is not a child of this zone or the last child).
	SemanticNode *nextChild(SemanticNode *child);

protected:
	virtual QLinkedList<SemanticNode *> children() const;
	QString toStringImpl(GeneratorCustomizer &customizer, int indent, const QString &indentString) const override;

private:
	QLinkedList<SemanticNode *> mChildren;
};

}
}
