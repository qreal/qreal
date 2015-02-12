#include "ifWithOneVisitedRule.h"

using namespace generatorBase::semantics;
using namespace qReal;

IfWithOneVisitedRule::IfWithOneVisitedRule(SemanticTree *tree, const Id &id
		, const LinkInfo &thenLink, const LinkInfo &elseLink)
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

	const LinkInfo &visitedLink = alreadyCreated(mThenLink) ? mThenLink : mElseLink;
	const LinkInfo &unvisitedLink = alreadyCreated(mThenLink) ? mElseLink : mThenLink;

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
