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

	virtual QString toString() const;

protected:
	virtual QLinkedList<SemanticNode *> children() const;

private:
	ZoneNode *mZone;  // Takes ownership
};

}
}
}
}
