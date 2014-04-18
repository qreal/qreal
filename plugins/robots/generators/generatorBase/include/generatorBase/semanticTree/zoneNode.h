#pragma once

#include "semanticNode.h"

namespace qReal {
namespace robots {
namespace generators {
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

	/// Removes all the children starting from specified node without deletting
	/// themselves and returns removed tail. Removes all if node is null.
	QLinkedList<SemanticNode *> removeStartingFrom(SemanticNode *node);


	/// Returns parent semantic node. The result is never NULL.
	SemanticNode *parentNode();

protected:
	virtual QLinkedList<SemanticNode *> children() const;
	QString toStringImpl(GeneratorCustomizer &customizer, int indent) const override;

private:
	QLinkedList<SemanticNode *> mChildren;
};

}
}
}
}
