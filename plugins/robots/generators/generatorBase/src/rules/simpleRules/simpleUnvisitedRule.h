#pragma once

#include "simpleBlockRuleBase.h"

namespace generatorBase {
namespace semantics {

/// This rule is applied to blocks with simple semantics when it is connected to the
/// unvisited node. The simplest case, just adding sibling to current semantic node.
class SimpleUnvisitedRule : public SimpleBlockRuleBase
{
public:
	SimpleUnvisitedRule(SemanticTree *tree, qReal::Id const &id, LinkInfo const &next);

	virtual bool apply();
};

}
}
