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

private:
	virtual void debugPrint(int indent);

	ZoneNode *mThenZone;
	ZoneNode *mElseZone;
};

}
}
}
}
