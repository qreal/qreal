#include "ifWithBothUnvisitedRule.h"

using namespace generatorBase::semantics;
using namespace qReal;

IfWithBothUnvisitedRule::IfWithBothUnvisitedRule(SemanticTree *tree, const Id &id
		, const LinkInfo &thenLink, const LinkInfo &elseLink)
	: IfBlockRuleBase(tree, id, thenLink, elseLink)
{
}

bool IfWithBothUnvisitedRule::apply()
{
	if (alreadyCreated(mThenLink) || alreadyCreated(mElseLink)) {
		return false;
	}

	IfNode * const thisNode = static_cast<IfNode *>(mTree->findNodeFor(mId));

	SemanticNode * const thenNode = mTree->produceNodeFor(mThenLink.target);
	thisNode->thenZone()->appendChild(thenNode);

	if (mThenLink.target == mElseLink.target) {
		thisNode->transformToSimple();
	} else {
		SemanticNode * const elseNode = mTree->produceNodeFor(mElseLink.target);
		thisNode->elseZone()->appendChild(elseNode);
	}

	return true;
}
