#include "mergedSwitchBranchesRule.h"

using namespace generatorBase::semantics;
using namespace qReal;

/// @todo: Unify this rule with if?

MergedSwitchBranchesRule::MergedSwitchBranchesRule(SemanticTree *tree
		, const Id &id, const LinkInfo &next)
	: SimpleVisitedRuleBase(tree, id, next)
{
}

bool MergedSwitchBranchesRule::apply()
{
	if (!SimpleVisitedRuleBase::apply()) {
		return false;
	}

	ZoneNode * const thisZone = mThisNode->parentZone();
	ZoneNode * const nextZone = mNextNode->parentZone();

	SwitchNode * const switchParent = dynamic_cast<SwitchNode *>(thisZone->parentNode());
	SwitchNode * const nextSwitchParent = dynamic_cast<SwitchNode *>(nextZone->parentNode());

	if (switchParent == nextSwitchParent && switchParent && !switchParent->branchesMerged() && thisZone != nextZone) {
		// Then we got merged branches case, handling it...
		QLinkedList<SemanticNode *> const detachedChildren = nextZone->removeStartingFrom(mNextNode);
		switchParent->appendSiblings(detachedChildren);
		switchParent->setBranchesMergedFlag();
		return true;
	}

	// If we then have next branch merging at the same point we can still consider this rule applied.
	return switchParent && switchParent->parentZone() == nextZone && nextZone->nextChild(switchParent) == mNextNode;
}
