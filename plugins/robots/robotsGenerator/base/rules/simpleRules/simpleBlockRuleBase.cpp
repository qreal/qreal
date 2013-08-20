#include "simpleBlockRuleBase.h"

using namespace qReal::robots::generators::semantics;

SimpleBlockRuleBase::SimpleBlockRuleBase(SemanticTree *tree
		, Id const &id, QObject *parent = 0)
	: SemanticTransformationRule(tree, id, parent)
	, mNext(next)
{
}
