#include "loopBlockRuleBase.h"

using namespace qReal::robots::generators::semantics;

LoopBlockRuleBase::LoopBlockRuleBase(SemanticTree *tree, Id const &id)
	: SemanticTransformationRule(tree, id)
{
}
