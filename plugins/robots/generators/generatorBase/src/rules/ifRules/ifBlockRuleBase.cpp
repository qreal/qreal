#include "ifBlockRuleBase.h"

using namespace generatorBase::semantics;
using namespace qReal;

IfBlockRuleBase::IfBlockRuleBase(SemanticTree *tree, const Id &id
		, const LinkInfo &thenLink, const LinkInfo &elseLink)
	: SemanticTransformationRule(tree, id)
	, mThenLink(thenLink)
	, mElseLink(elseLink)
{
}
