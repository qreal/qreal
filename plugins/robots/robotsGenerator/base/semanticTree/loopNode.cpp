#include "loopNode.h"

using namespace qReal::robots::generators::semantics;

LoopNode::LoopNode(Id const &idBinded, QObject *parent)
	: SemanticNode(idBinded, parent)
	, mBodyZone(new ZoneNode(this))
{
	mBodyZone->setParentNode(this);
}

QString LoopNode::toString() const
{
	return "loop";
}

QLinkedList<SemanticNode *> LoopNode::children() const
{
	return QLinkedList<SemanticNode *>() << mBodyZone;
}
