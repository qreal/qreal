#include "loopBlockRuleBase.h"

using namespace generatorBase::semantics;
using namespace qReal;

LoopBlockRuleBase::LoopBlockRuleBase(SemanticTree *tree, const Id &id
		, const LinkInfo &iterationLink, const LinkInfo &nextLink)
	: SemanticTransformationRule(tree, id)
	, mIterationLink(iterationLink)
	, mNextLink(nextLink)
{
}
