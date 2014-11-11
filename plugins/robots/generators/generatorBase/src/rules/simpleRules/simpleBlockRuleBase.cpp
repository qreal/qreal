#include "simpleBlockRuleBase.h"

using namespace generatorBase::semantics;
using namespace qReal;

SimpleBlockRuleBase::SimpleBlockRuleBase(SemanticTree *tree, Id const &id
		, LinkInfo const &next)
	: SemanticTransformationRule(tree, id)
	, mNextLink(next)
{
}
