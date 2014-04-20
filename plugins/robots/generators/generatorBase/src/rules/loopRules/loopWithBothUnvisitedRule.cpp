#include "loopWithBothUnvisitedRule.h"

using namespace generatorBase::semantics;
using namespace qReal;

LoopWithBothUnvisitedRule::LoopWithBothUnvisitedRule(SemanticTree *tree, Id const &id
		, LinkInfo const &iterationLink, LinkInfo const &nextLink)
	: LoopBlockRuleBase(tree, id, iterationLink, nextLink)
{
}

bool LoopWithBothUnvisitedRule::apply()
{
	if (alreadyCreated(mIterationLink) || alreadyCreated(mNextLink)) {
		return false;
	}

	LoopNode * const thisNode = static_cast<LoopNode *>(mTree->findNodeFor(mId));
	SemanticNode * const iterationNode = mTree->produceNodeFor(mIterationLink.target);
	SemanticNode * const nextNode = mTree->produceNodeFor(mNextLink.target);

	thisNode->bodyZone()->appendChild(iterationNode);
	thisNode->appendSibling(nextNode);

	return true;
}
