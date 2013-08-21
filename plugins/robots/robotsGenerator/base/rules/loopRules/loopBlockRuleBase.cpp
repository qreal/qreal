#include "loopBlockRuleBase.h"

using namespace qReal::robots::generators::semantics;

LoopBlockRuleBase::LoopBlockRuleBase(SemanticTree *tree, Id const &id
		, LinkInfo const &iterationNode, LinkInfo const &nextNode)
	: SemanticTransformationRule(tree, id)
	, mIterationLink(iterationNode)
	, mNextLink(nextNode)
{
}
