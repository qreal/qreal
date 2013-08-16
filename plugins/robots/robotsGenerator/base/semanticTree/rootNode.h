#pragma once

#include "semanticNode.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class RootNode : public SemanticNode
{
public:
	explicit RootNode(QObject *parent = 0);
};

}
}
}
}
