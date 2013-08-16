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
	ZoneNode *mIfZone;
	ZoneNode *mElseZone;
};

}
}
}
}
