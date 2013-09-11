#pragma once

#include "semanticNode.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class ZoneNode : public SemanticNode
{
public:
	explicit ZoneNode(QObject *parent = 0);

	bool isEmpty() const;

	void appendChild(SemanticNode *node);

	void appendChildren(QLinkedList<SemanticNode *> const &nodes);

	/// Removes given node from children list without deleting the child itself
	void removeChild(SemanticNode *node);

	/// Removes last node from children list without deleting the child itself
	SemanticNode *removeLast();

	/// Removes all the children starting from specified node without deletting
	/// themselves and returns removed tail. Removes all if node is null.
	QLinkedList<SemanticNode *> removeStartingFrom(SemanticNode *node);

	virtual QString toString(GeneratorCustomizer &customizer, int indent) const;

	SemanticNode *parentNode();

protected:
	virtual QLinkedList<SemanticNode *> children() const;

private:
	QLinkedList<SemanticNode *> mChildren;
};

}
}
}
}
