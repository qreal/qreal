#include "simpleVisitedOneZoneRule.h"
#include "../../semanticTree/loopNode.h"

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
		return false;
	}

	LoopNode *endlessLoop = mTree->produceLoop();
	ZoneNode *parent = mThisNode->parentZone();

	QLinkedList<SemanticNode *> const unhookedChildren = parent->removeStartingFrom(mNextNode);
	endlessLoop->appendChildren(unhookedChildren);

	parent->appendChild(endlessLoop);

	return true;
}
