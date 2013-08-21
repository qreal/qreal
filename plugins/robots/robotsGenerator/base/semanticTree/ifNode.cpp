#include "ifNode.h"

using namespace qReal::robots::generators::semantics;

IfNode::IfNode(Id const &idBinded, QObject *parent)
	: NonZoneNode(idBinded, parent)
	, mThenZone(new ZoneNode(this))
	, mElseZone(new ZoneNode(this))
	, mAddNotToCondition(false)
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

QString IfNode::toString() const
{
	return "if";
}

QLinkedList<SemanticNode *> IfNode::children() const
{
	return QLinkedList<SemanticNode *>() << mThenZone << mElseZone;
}
