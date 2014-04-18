#pragma once

#include "zoneNode.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

/// A base for all semantic nodes except zones
class NonZoneNode : public SemanticNode
{
public:
	explicit NonZoneNode(Id const &idBinded, QObject *parent = 0);

	ZoneNode *parentZone();

	void appendSibling(SemanticNode *node);
	void appendSiblings(QLinkedList<SemanticNode *> const &nodes);
	void insertSiblingAfterThis(SemanticNode *node);
};

}
}
}
}
