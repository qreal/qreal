#pragma once

#include "ifBlockRuleBase.h"

namespace generatorBase {
namespace semantics {

/// This rule is applied to blocks with if semantics with both branches unvisited.
/// Simply prepares if semantic node with two regular branches.
class IfWithBothUnvisitedRule : public IfBlockRuleBase
{
public:
	IfWithBothUnvisitedRule(SemanticTree *tree, qReal::Id const &id
			, LinkInfo const &thenLink, LinkInfo const &elseLink);

	virtual bool apply();
};

}
}
