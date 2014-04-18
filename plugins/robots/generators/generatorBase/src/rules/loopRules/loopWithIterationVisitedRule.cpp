#include "loopWithIterationVisitedRule.h"

using namespace qReal::robots::generators::semantics;

LoopWithIterationVisitedRule::LoopWithIterationVisitedRule(SemanticTree *tree
		, Id const &id, LinkInfo const &iterationLink, LinkInfo const &nextLink)
	: LoopBlockRuleBase(tree, id, iterationLink, nextLink)
{
}

bool LoopWithIterationVisitedRule::apply()
{
	if (!alreadyCreated(mIterationLink) || alreadyCreated(mNextLink)) {
		return false;
	}

	LoopNode * const thisNode = static_cast<LoopNode *>(mTree->findNodeFor(mId));
	NonZoneNode * const iterationNode = mTree->findNodeFor(mIterationLink.target);

	if (thisNode->parentZone() != iterationNode->parentZone()) {
		return false;
	}

	LoopNode * const newLoop = makeLoopStartingFrom(iterationNode);
	newLoop->bodyZone()->removeChild(thisNode);
	newLoop->bindTo(mId);
	newLoop->setForm(true);

	SemanticNode * const nextNode = mTree->produceNodeFor(mNextLink.target);
	newLoop->appendSibling(nextNode);

	return true;
}
