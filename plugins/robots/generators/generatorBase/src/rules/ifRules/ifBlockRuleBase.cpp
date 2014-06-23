#include "ifBlockRuleBase.h"

using namespace generatorBase::semantics;
using namespace qReal;

IfBlockRuleBase::IfBlockRuleBase(SemanticTree *tree, Id const &id
		, LinkInfo const &thenLink, LinkInfo const &elseLink)
	: SemanticTransformationRule(tree, id)
	, mThenLink(thenLink)
	, mElseLink(elseLink)
{
}
