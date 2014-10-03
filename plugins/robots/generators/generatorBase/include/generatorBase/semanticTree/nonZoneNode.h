#pragma once

#include "zoneNode.h"

namespace generatorBase {
namespace semantics {

/// A base for all semantic nodes except zones
class NonZoneNode : public SemanticNode
{
public:
	explicit NonZoneNode(qReal::Id const &idBinded, QObject *parent = 0);

	ZoneNode *parentZone() const;

	void appendSibling(SemanticNode *node);
	void appendSiblings(QLinkedList<SemanticNode *> const &nodes);
	void insertSiblingAfterThis(SemanticNode *node);
};

}
}
