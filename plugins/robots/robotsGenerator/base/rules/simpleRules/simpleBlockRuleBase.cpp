#include "simpleBlockRuleBase.h"

using namespace qReal::robots::generators::semantics;

SimpleBlockRuleBase::SimpleBlockRuleBase(SemanticTree *tree, Id const &id
		, LinkInfo const &next, QObject *parent)
	: SemanticTransformationRule(tree, id, parent)
	, mNext(next)
{
}
