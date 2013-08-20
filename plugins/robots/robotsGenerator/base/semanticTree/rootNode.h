#pragma once

#include "zoneNode.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class RootNode : public SemanticNode
{
public:
	RootNode(Id const &initialBlock, QObject *parent = 0);

private:
	virtual void debugPrint(int indent);

	ZoneNode *mZone;  // Takes ownership
};

}
}
}
}
