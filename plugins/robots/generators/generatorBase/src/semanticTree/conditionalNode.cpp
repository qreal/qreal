#include "generatorBase/semanticTree/conditionalNode.h"

using namespace generatorBase::semantics;
using namespace qReal;

ConditionalNode::ConditionalNode(Id const &idBinded, QObject *parent)
	: NonZoneNode(idBinded, parent)
	, mAddNotToCondition(false)
{
}

void ConditionalNode::invertCondition()
{
	mAddNotToCondition = !mAddNotToCondition;
}
