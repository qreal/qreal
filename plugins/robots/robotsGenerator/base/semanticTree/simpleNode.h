#pragma once

#include "nonZoneNode.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class SimpleNode : public NonZoneNode
{
public:
	SimpleNode(Id const &idBinded, QObject *parent = 0);

	virtual QString toString() const;

protected:
	virtual QLinkedList<SemanticNode *> children() const;
};

}
}
}
}
