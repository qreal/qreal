#include "ifNode.h"

using namespace qReal::robots::generators::semantics;

IfNode::IfNode(Id const &idBinded, QObject *parent)
	: SemanticNode(idBinded, parent)
	, mThenZone(new ZoneNode(this))
	, mElseZone(new ZoneNode(this))
{
	mThenZone->setParentNode(this);
	mElseZone->setParentNode(this);
}

QString IfNode::toString() const
{
	return "if";
}

QLinkedList<SemanticNode *> IfNode::children() const
{
	return QLinkedList<SemanticNode *>() << mThenZone << mElseZone;
}
