#include "loopBlockRuleBase.h"

using namespace qReal::robots::generators::semantics;

LoopBlockRuleBase::LoopBlockRuleBase(SemanticTree *tree, Id const &id
		, LinkInfo const &iterationLink, LinkInfo const &nextLink)
	: SemanticTransformationRule(tree, id)
	, mIterationLink(iterationLink)
	, mNextLink(nextLink)
{
}
