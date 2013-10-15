#pragma once

#include "ifBlockRuleBase.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

/// This rule is applied to blocks with if semantics with both branches unvisited.
/// Simply prepares if semantic node with two regular branches.
class IfWithBothUnvisitedRule : public IfBlockRuleBase
{
public:
	IfWithBothUnvisitedRule(SemanticTree *tree, Id const &id
			, LinkInfo const &thenLink, LinkInfo const &elseLink);

	virtual bool apply();
};

}
}
}
}
