#include "ifNode.h"

using namespace qReal::robots::generators::semantics;

IfNode::IfNode(Id const &idBinded, QObject *parent)
	: ConditionalNode(idBinded, parent)
	, mThenZone(new ZoneNode(this))
	, mElseZone(new ZoneNode(this))
{
	mThenZone->setParentNode(this);
	mElseZone->setParentNode(this);
}

ZoneNode *IfNode::parentZone()
{
	return static_cast<ZoneNode *>(mParentNode);
}

ZoneNode *IfNode::thenZone()
{
	return mThenZone;
}

ZoneNode *IfNode::elseZone()
{
	return mElseZone;
}

QString IfNode::toString(GeneratorCustomizer &customizer) const
{
	return QString("if") + ConditionalNode::toString(customizer);
}

QLinkedList<SemanticNode *> IfNode::children() const
{
	return QLinkedList<SemanticNode *>() << mThenZone << mElseZone;
}
