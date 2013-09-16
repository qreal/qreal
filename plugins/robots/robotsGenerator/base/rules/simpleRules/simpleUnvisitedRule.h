#pragma once

#include "simpleBlockRuleBase.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

/// This rule is applied to blocks with simple semantics when it is connected to the
/// unvisited node. The simplest case, just adding sibling to current semantic node.
class SimpleUnvisitedRule : public SimpleBlockRuleBase
{
public:
	SimpleUnvisitedRule(SemanticTree *tree, Id const &id, LinkInfo const &next);

	virtual bool apply();
};

}
}
}
}
