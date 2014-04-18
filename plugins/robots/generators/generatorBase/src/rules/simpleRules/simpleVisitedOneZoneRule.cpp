#include "simpleVisitedOneZoneRule.h"

#include "generatorBase/semanticTree/loopNode.h"

using namespace qReal::robots::generators::semantics;

SimpleVisitedOneZoneRule::SimpleVisitedOneZoneRule(SemanticTree *tree, Id const &id
		, LinkInfo const &next)
	: SimpleVisitedRuleBase(tree, id, next)
{
}

bool SimpleVisitedOneZoneRule::apply()
{
	if (!SimpleVisitedRuleBase::apply()) {
		return false;
	}

	if (mNextNode->parentZone() != mThisNode->parentZone()) {
		// If we have backward edge to loop - it`s ok, simply ignoring it
		return mThisNode->parentZone()->parentNode() == mNextNode
				&& dynamic_cast<LoopNode *>(mNextNode);
	}

	makeLoopStartingFrom(mNextNode);

	return true;
}
