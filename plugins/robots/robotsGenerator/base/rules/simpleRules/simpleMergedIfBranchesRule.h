#pragma once

#include "simpleVisitedRuleBase.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

/// This rule is applied to blocks with simple semantics when it leads to a coupled
/// branch of the parent if statement ('merged' branches case). Attaches tail
/// of the branch to if`s siblings.
class SimpleMergedIfBranchesRule : public SimpleVisitedRuleBase
{
public:
	SimpleMergedIfBranchesRule(SemanticTree *tree, Id const &id, LinkInfo const &next);

	virtual bool apply();
};

}
}
}
}
