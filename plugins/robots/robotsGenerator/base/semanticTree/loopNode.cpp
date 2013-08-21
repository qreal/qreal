#include "loopNode.h"

using namespace qReal::robots::generators::semantics;

LoopNode::LoopNode(Id const &idBinded, QObject *parent)
	: NonZoneNode(idBinded, parent)
	, mBodyZone(new ZoneNode(this))
{
	mBodyZone->setParentNode(this);
}

QString LoopNode::toString() const
{
	return "loop";
}

void LoopNode::appendChildren(QLinkedList<SemanticNode *> const &nodes)
{
	mBodyZone->appendChildren(nodes);
}

QLinkedList<SemanticNode *> LoopNode::children() const
{
	return QLinkedList<SemanticNode *>() << mBodyZone;
}
