#pragma once

#include "zoneNode.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class IfNode : public SemanticNode
{
public:
	IfNode(Id const &idBinded, QObject *parent = 0);

	virtual QString toString() const;

protected:
	virtual QLinkedList<SemanticNode *> children() const;

private:
	ZoneNode *mThenZone;  // Takes ownership
	ZoneNode *mElseZone;  // Takes ownership
};

}
}
}
}
