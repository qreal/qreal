#pragma once

#include "semanticNode.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class SimpleNode : public SemanticNode
{
public:
	SimpleNode(Id const &idBinded, QObject *parent = 0);

private:
	virtual void debugPrint(int indent);
};

}
}
}
}
