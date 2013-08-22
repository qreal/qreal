#include "ifWithBothUnvisitedRule.h"

using namespace qReal::robots::generators::semantics;

IfWithBothUnvisitedRule::IfWithBothUnvisitedRule(SemanticTree *tree, Id const &id
		, LinkInfo const &thenLink, LinkInfo const &elseLink)
	: IfBlockRuleBase(tree, id, thenLink, elseLink)
{
}

bool IfWithBothUnvisitedRule::apply()
{
	if (mThenLink.targetVisited || mElseLink.targetVisited) {
		return false;
	}

	SemanticNode * const thenNode = mTree->produceNodeFor(mThenLink.target);
	SemanticNode * const elseNode = mTree->produceNodeFor(mElseLink.target);

	IfNode * const thisNode = static_cast<IfNode *>(mTree->findNodeFor(mId));
	thisNode->thenZone()->appendChild(thenNode);
	thisNode->elseZone()->appendChild(elseNode);
}
