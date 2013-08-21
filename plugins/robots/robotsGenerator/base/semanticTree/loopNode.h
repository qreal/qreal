#pragma once

#include "zoneNode.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class LoopNode : public SemanticNode
{
public:
	LoopNode(Id const &idBinded, QObject *parent = 0);

	virtual QString toString() const;

protected:
	virtual QLinkedList<SemanticNode *> children() const;

private:
	ZoneNode *mBodyZone;  // Takes ownership
};

}
}
}
}
