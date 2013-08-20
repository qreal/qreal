#include "semanticTransformationRule.h"

using namespace qReal::robots::generators::semantics;

SemanticTransformationRule::SemanticTransformationRule(SemanticTree *tree
		, Id const &id, QObject *parent)
	: QObject(parent)
	, mTree(tree)
	, mId(id)
{
}

SemanticTransformationRule::~SemanticTransformationRule()
{
}
