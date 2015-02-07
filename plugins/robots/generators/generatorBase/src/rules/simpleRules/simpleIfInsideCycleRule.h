#pragma once

#include "simpleVisitedRuleBase.h"

namespace generatorBase {
namespace semantics {

/// This rule is applied to blocks with simple semantics when it creates infinite cycle
/// but immediate parent of the current block is a condition with one branch leading
/// out of the mentioned cycle. Transforms parent if statement into then-break branch.
class SimpleIfInsideCycleRule : public SimpleVisitedRuleBase
{
public:
	SimpleIfInsideCycleRule(SemanticTree *tree, const qReal::Id &id, const LinkInfo &next);

	virtual bool apply();
};

}
}
