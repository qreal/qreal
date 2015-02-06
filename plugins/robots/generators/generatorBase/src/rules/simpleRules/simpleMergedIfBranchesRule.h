#pragma once

#include "simpleVisitedRuleBase.h"

namespace generatorBase {
namespace semantics {

/// This rule is applied to blocks with simple semantics when it leads to a coupled
/// branch of the parent if statement ('merged' branches case). Attaches tail
/// of the branch to if`s siblings.
class SimpleMergedIfBranchesRule : public SimpleVisitedRuleBase
{
public:
	SimpleMergedIfBranchesRule(SemanticTree *tree, const qReal::Id &id, LinkInfo const &next);

	virtual bool apply();
};

}
}
