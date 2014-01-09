#pragma once

#include "ifBlockRuleBase.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

/// This rule is applied to blocks with if semantics with exactly one branch visited.
/// If branch comes into the node in the same with current if block zone creates
/// cycle with conditional break, otherwise this rule is considered non-applicable.
class IfWithOneVisitedRule : public IfBlockRuleBase
{
public:
	IfWithOneVisitedRule(SemanticTree *tree, Id const &id
			, LinkInfo const &thenLink, LinkInfo const &elseLink);

	virtual bool apply();
};

}
}
}
}
