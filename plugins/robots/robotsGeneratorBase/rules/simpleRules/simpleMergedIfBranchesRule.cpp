#include "simpleMergedIfBranchesRule.h"

using namespace qReal::robots::generators::semantics;

SimpleMergedIfBranchesRule::SimpleMergedIfBranchesRule(SemanticTree *tree
		, Id const &id, LinkInfo const &next)
	: SimpleVisitedRuleBase(tree, id, next)
{
}

bool SimpleMergedIfBranchesRule::apply()
{
	if (!SimpleVisitedRuleBase::apply()) {
		return false;
	}

	ZoneNode * const thisZone = mThisNode->parentZone();
	ZoneNode * const nextZone = mNextNode->parentZone();

	IfNode *ifParent = dynamic_cast<IfNode *>(thisZone->parentNode());
	IfNode *nextIfParent = dynamic_cast<IfNode *>(nextZone->parentNode());

	if (ifParent != nextIfParent || !ifParent || thisZone == nextZone) {
		return false;
	}

	QLinkedList<SemanticNode *> const detachedChildren = nextZone->removeStartingFrom(mNextNode);
	ifParent->appendSiblings(detachedChildren);

	return true;
}
