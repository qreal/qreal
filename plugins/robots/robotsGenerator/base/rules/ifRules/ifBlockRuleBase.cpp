#include "ifBlockRuleBase.h"

using namespace qReal::robots::generators::semantics;

IfBlockRuleBase::IfBlockRuleBase(SemanticTree *tree, Id const &id, QObject *parent)
	: SemanticTransformationRule(tree, id, parent)
{
}
