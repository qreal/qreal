#include "ifBlockRuleBase.h"

using namespace qReal::robots::generators::semantics;

IfBlockRuleBase::IfBlockRuleBase(SemanticTree *tree, Id const &id
		, LinkInfo const &thenLink, LinkInfo const &elseLink)
	: SemanticTransformationRule(tree, id)
	, mThenLink(thenLink)
	, mElseLink(elseLink)
{
}
