#include "simpleUnvisitedRule.h"
#include "../../semanticTree/simpleNode.h"

using namespace qReal::robots::generators::semantics;

SimpleUnvisitedRule::SimpleUnvisitedRule(SemanticTree *tree, Id const &id
		, LinkInfo const &next)
	: SimpleBlockRuleBase(tree, id, next)
{
}

bool SimpleUnvisitedRule::apply()
{
	if (alreadyCreated(mNextLink)) {
		return false;
	}

	SemanticNode * const nextNode = mTree->produceNodeFor(mNextLink.target);
	SimpleNode * const thisNode = static_cast<SimpleNode *>(mTree->findNodeFor(mId));
	thisNode->appendSibling(nextNode);

	return true;
}
