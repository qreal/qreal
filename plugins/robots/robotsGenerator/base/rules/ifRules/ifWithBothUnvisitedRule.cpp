#include "ifWithBothUnvisitedRule.h"

using namespace qReal::robots::generators::semantics;

IfWithBothUnvisitedRule::IfWithBothUnvisitedRule(SemanticTree *tree, Id const &id
		, LinkInfo const &thenNode, LinkInfo const &elseNode)
	: IfBlockRuleBase(tree, id, thenNode, elseNode)
{
}

bool IfWithBothUnvisitedRule::apply()
{
	if (mThenLink.targetVisited || mElseLink.targetVisited) {
		return false;
	}

	SemanticNode *thenNode = mTree->produceNodeFor(mThenLink.target);
	SemanticNode *elseNode = mTree->produceNodeFor(mElseLink.target);

	IfNode *thisNode = static_cast<IfNode *>(mTree->findNodeFor(mId));
	thisNode->appendThenBranch(thenNode);
	thisNode->appendElseBranch(elseNode);
}
