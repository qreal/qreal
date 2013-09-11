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

	if (mThenLink.targetVisited) {
		++visitedCount;
	}

	if (mElseLink.targetVisited) {
		++visitedCount;
	}

	if (visitedCount != 1) {
		return false;
	}

	IfNode *thisNode = static_cast<IfNode *>(mTree->findNodeFor(mId));

	LinkInfo const &visitedLink = mThenLink.targetVisited ? mThenLink : mElseLink;
	LinkInfo const &unvisitedLink = mThenLink.targetVisited ? mElseLink : mThenLink;

	NonZoneNode *nextNode = mTree->findNodeFor(visitedLink.target);
	if (nextNode->parentZone() != thisNode->parentZone()) {
		return false;
	}

	LoopNode *loop = makeLoopStartingFrom(nextNode);
	loop->bodyZone()->removeChild(thisNode);
	loop->bindTo(mId);
	loop->setForm(true);
	if (mElseLink.targetVisited) {
		loop->invertCondition();
	}

	SemanticNode *newNode = mTree->produceNodeFor(unvisitedLink.target);
	loop->appendSibling(newNode);

	return true;
}
