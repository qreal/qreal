#include "generatorBase/semanticTree/conditionalNode.h"

using namespace qReal::robots::generators::semantics;

ConditionalNode::ConditionalNode(Id const &idBinded, QObject *parent)
	: NonZoneNode(idBinded, parent)
	, mAddNotToCondition(false)
{
}

void ConditionalNode::invertCondition()
{
	mAddNotToCondition = !mAddNotToCondition;
}
