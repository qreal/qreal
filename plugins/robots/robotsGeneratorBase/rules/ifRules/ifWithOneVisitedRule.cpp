#include "ifWithOneVisitedRule.h"

using namespace qReal::robots::generators::semantics;

IfWithOneVisitedRule::IfWithOneVisitedRule(SemanticTree *tree, Id const &id
		, LinkInfo const &thenLink, LinkInfo const &elseLink)
	: IfBlockRuleBase(tree, id, thenLink, elseLink)
{
}

bool IfWithOneVisitedRule::apply()
{
	int visitedCount = 0;

	if (alreadyCreated(mThenLink)) {
		++visitedCount;
	}

	if (alreadyCreated(mElseLink)) {
		++visitedCount;
	}

	if (visitedCount != 1) {
		return false;
	}

	LinkInfo const &visitedLink = alreadyCreated(mThenLink) ? mThenLink : mElseLink;
	LinkInfo const &unvisitedLink = alreadyCreated(mThenLink) ? mElseLink : mThenLink;

	IfNode * const thisNode = static_cast<IfNode *>(mTree->findNodeFor(mId));
	SemanticNode *newNode = mTree->produceNodeFor(unvisitedLink.target);

	NonZoneNode * const nextNode = mTree->findNodeFor(visitedLink.target);
	if (nextNode->parentZone() != thisNode->parentZone()) {
		(alreadyCreated(mThenLink) ? thisNode->elseZone() : thisNode->thenZone())->appendChild(newNode);
		return false;
	}

	LoopNode * const loop = makeLoopStartingFrom(nextNode);
	loop->bodyZone()->removeChild(thisNode);
	loop->bindTo(mId);
	loop->setForm(true);
	if (alreadyCreated(mElseLink)) {
		loop->invertCondition();
	}

	loop->appendSibling(newNode);

	return true;
}
