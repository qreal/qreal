#include "simpleIfInsideCycleRule.h"

using namespace generatorBase::semantics;
using namespace qReal;

SimpleIfInsideCycleRule::SimpleIfInsideCycleRule(SemanticTree *tree
		, Id const &id, LinkInfo const &next)
	: SimpleVisitedRuleBase(tree, id, next)
{
}

bool SimpleIfInsideCycleRule::apply()
{
	if (!SimpleVisitedRuleBase::apply()) {
		return false;
	}

	IfNode * const parentIf = dynamic_cast<IfNode *>(mThisNode->parentZone()->parentNode());
	if (!parentIf || parentIf->parentZone() != mNextNode->parentZone()) {
		return false;
	}

	LoopNode * const parentLoop = makeLoopStartingFrom(mNextNode);

	ZoneNode * const thisZone = mThisNode->parentZone();
	ZoneNode * const otherZone = parentIf->thenZone() == thisZone
			? parentIf->elseZone() : parentIf->thenZone();

	QLinkedList<SemanticNode *> const detachedThisBranch = thisZone->removeStartingFrom(NULL);
	QLinkedList<SemanticNode *> const detachedOtherBranch = otherZone->removeStartingFrom(NULL);

	parentIf->appendSiblings(detachedThisBranch);
	parentLoop->appendSiblings(detachedOtherBranch);

	if (thisZone == parentIf->thenZone()) {
		parentIf->invertCondition();
	}

	SimpleNode * const breakNode = mTree->produceSimple(Id());
	breakNode->bindToSyntheticConstruction(SimpleNode::breakNode);
	parentIf->thenZone()->appendChild(breakNode);

	return true;
}
