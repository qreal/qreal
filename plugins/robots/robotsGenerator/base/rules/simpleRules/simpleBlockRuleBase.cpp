#include "simpleBlockRuleBase.h"

using namespace qReal::robots::generators::semantics;

SimpleBlockRuleBase::SimpleBlockRuleBase(SemanticTree *tree, Id const &id
		, LinkInfo const &next)
	: SemanticTransformationRule(tree, id)
	, mNextLink(next)
{
}
