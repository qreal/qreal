#pragma once

#include "nonZoneNode.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class LoopNode : public NonZoneNode
{
public:
	LoopNode(Id const &idBinded, QObject *parent = 0);

	virtual QString toString() const;

	void appendChildren(QLinkedList<SemanticNode *> const &nodes);

protected:
	virtual QLinkedList<SemanticNode *> children() const;

private:
	ZoneNode *mBodyZone;  // Takes ownership
};

}
}
}
}
