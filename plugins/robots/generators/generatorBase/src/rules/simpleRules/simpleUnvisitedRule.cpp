#include "simpleUnvisitedRule.h"
#include "generatorBase/semanticTree/simpleNode.h"

using namespace generatorBase::semantics;
using namespace qReal;

SimpleUnvisitedRule::SimpleUnvisitedRule(SemanticTree *tree, const Id &id
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
