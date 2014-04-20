#include "loopBlockRuleBase.h"

using namespace generatorBase::semantics;
using namespace qReal;

LoopBlockRuleBase::LoopBlockRuleBase(SemanticTree *tree, Id const &id
		, LinkInfo const &iterationLink, LinkInfo const &nextLink)
	: SemanticTransformationRule(tree, id)
	, mIterationLink(iterationLink)
	, mNextLink(nextLink)
{
}
