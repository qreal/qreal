#include "simpleUnvisitedRule.h"
#include "../../semanticTree/simpleNode.h"

using namespace qReal::robots::generators::semantics;

SimpleUnvisitedRule::SimpleUnvisitedRule(SemanticTree *tree, Id const &id
		, LinkInfo const &next, QObject *parent)
	: SimpleBlockRuleBase(tree, id, next, parent)
{
}

bool SimpleUnvisitedRule::canApply()
{
	return !mNext.targetVisited;
}

void SimpleUnvisitedRule::apply()
{
	SemanticNode *nextNode = mTree->produceNodeFor(mNext.target);
	SimpleNode *thisNode = static_cast<SimpleNode *>(findNodeInSemanticTree());
	thisNode->appendSibling(nextNode);
}
