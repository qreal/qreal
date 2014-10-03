#pragma once

#include "src/rules/simpleRules/simpleVisitedRuleBase.h"

namespace generatorBase {
namespace semantics {

/// This rule is applied to blocks with simple semantics when it leads to other
/// branch of the parent switch statement ('merged' branches case). Attaches tail
/// of the branch to switch`s siblings. Also handles case when link leads to the
/// switch block`s immediate sibling (when two branches were already merged),
/// terminating current branch in that case.
class MergedSwitchBranchesRule : public SimpleVisitedRuleBase
{
public:
	MergedSwitchBranchesRule(SemanticTree *tree, qReal::Id const &id, LinkInfo const &next);

	bool apply() override;
};

}
}
