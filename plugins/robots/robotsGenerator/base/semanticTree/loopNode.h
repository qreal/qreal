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

private:
	ZoneNode *mBodyZone;
};

}
}
}
}
