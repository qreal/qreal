#pragma once

#include "loopBlockRuleBase.h"

namespace generatorBase {
namespace semantics {

/// This rule is applied to blocks with loop semantics with both branches unvisited.
/// Simply prepares loop semantic node with two regular branches.
class LoopWithBothUnvisitedRule : public LoopBlockRuleBase
{
public:
	LoopWithBothUnvisitedRule(SemanticTree *tree, qReal::Id const &id
			, LinkInfo const &iterationLink, LinkInfo const &nextLink);

	virtual bool apply();
};

}
}
