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

	void appendChild(SemanticNode *node);
	/// Removes given node from child list without deleting the child itself
	void removeChild(SemanticNode *node);
	SemanticNode *removeLast();

	virtual QString toString() const;

protected:
	virtual QLinkedList<SemanticNode *> children() const;

private:
	QLinkedList<SemanticNode *> mChildren;
};

}
}
}
}
