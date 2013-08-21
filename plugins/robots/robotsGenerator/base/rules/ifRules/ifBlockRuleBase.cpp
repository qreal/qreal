#include "ifBlockRuleBase.h"

using namespace qReal::robots::generators::semantics;

IfBlockRuleBase::IfBlockRuleBase(SemanticTree *tree, Id const &id)
	: SemanticTransformationRule(tree, id)
{
}
