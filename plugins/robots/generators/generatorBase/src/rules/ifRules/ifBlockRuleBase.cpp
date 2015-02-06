#include "ifBlockRuleBase.h"

using namespace generatorBase::semantics;
using namespace qReal;

IfBlockRuleBase::IfBlockRuleBase(SemanticTree *tree, const Id &id
		, LinkInfo const &thenLink, LinkInfo const &elseLink)
	: SemanticTransformationRule(tree, id)
	, mThenLink(thenLink)
	, mElseLink(elseLink)
{
}
