#include "ifNode.h"

using namespace qReal::robots::generators::semantics;

IfNode::IfNode(Id const &idBinded, QObject *parent)
	: NonZoneNode(idBinded, parent)
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

QString IfNode::toString() const
{
	return "if";
}

void IfNode::appendThenBranch(SemanticNode *node)
{
	mThenZone->appendChild(node);
}

void IfNode::appendElseBranch(SemanticNode *node)
{
	mElseZone->appendChild(node);
}

QLinkedList<SemanticNode *> IfNode::children() const
{
	return QLinkedList<SemanticNode *>() << mThenZone << mElseZone;
}
