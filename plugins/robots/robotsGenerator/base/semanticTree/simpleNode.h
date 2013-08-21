#pragma once

#include "semanticNode.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class ZoneNode;

class SimpleNode : public SemanticNode
{
public:
	SimpleNode(Id const &idBinded, QObject *parent = 0);

	void appendSibling(SemanticNode *node);

	virtual QString toString() const;

protected:
	virtual QLinkedList<SemanticNode *> children() const;
};

}
}
}
}
