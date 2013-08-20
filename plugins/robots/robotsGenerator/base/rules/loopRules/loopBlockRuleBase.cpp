#include "loopBlockRuleBase.h"

using namespace qReal::robots::generators::semantics;

LoopBlockRuleBase::LoopBlockRuleBase(SemanticTree *tree, Id const &id, QObject *parent)
	: SemanticTransformationRule(tree, id, parent)
{
}
