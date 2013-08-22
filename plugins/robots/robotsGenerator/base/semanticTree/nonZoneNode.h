#pragma once

#include "zoneNode.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class NonZoneNode : public SemanticNode
{
public:
	NonZoneNode(Id const &idBinded, QObject *parent = 0);

	ZoneNode *parentZone();

	void appendSibling(SemanticNode *node);
	void appendSiblings(QLinkedList<SemanticNode *> const &nodes);
};

}
}
}
}
