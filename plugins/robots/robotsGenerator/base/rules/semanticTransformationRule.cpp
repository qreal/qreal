#include "semanticTransformationRule.h"

using namespace qReal::robots::generators::semantics;

SemanticTransformationRule::SemanticTransformationRule(SemanticTree *tree, Id const &id)
	: mTree(tree)
	, mId(id)
{
}

SemanticTransformationRule::~SemanticTransformationRule()
{
}
