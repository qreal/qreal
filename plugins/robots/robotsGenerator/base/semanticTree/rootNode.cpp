#include "rootNode.h"
#include "simpleNode.h"

using namespace qReal::robots::generators::semantics;

RootNode::RootNode(Id const &initialBlock, QObject *parent)
	: SemanticNode(Id(), parent)
	, mZone(new ZoneNode(this))
{
	mZone->setParentNode(this);
	mZone->appendChild(new SimpleNode(initialBlock, mZone));
}

QString RootNode::toString() const
{
	return "begin";
}

QLinkedList<SemanticNode *> RootNode::children() const
{
	return QLinkedList<SemanticNode *>() << mZone;
}
