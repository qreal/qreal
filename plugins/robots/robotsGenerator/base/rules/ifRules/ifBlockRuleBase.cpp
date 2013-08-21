#include "ifBlockRuleBase.h"

using namespace qReal::robots::generators::semantics;

IfBlockRuleBase::IfBlockRuleBase(SemanticTree *tree, Id const &id
		, LinkInfo const &thenNode, LinkInfo const &elseNode)
	: SemanticTransformationRule(tree, id)
	, mThenLink(thenNode)
	, mElseLink(elseNode)
{
}
