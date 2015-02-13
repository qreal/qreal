#include "generatorBase/semanticTree/conditionalNode.h"

using namespace generatorBase::semantics;
using namespace qReal;

ConditionalNode::ConditionalNode(const Id &idBinded, QObject *parent)
	: NonZoneNode(idBinded, parent)
	, mAddNotToCondition(false)
{
}

void ConditionalNode::invertCondition()
{
	mAddNotToCondition = !mAddNotToCondition;
}
