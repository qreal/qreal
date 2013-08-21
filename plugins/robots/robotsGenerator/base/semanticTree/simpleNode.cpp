#include "simpleNode.h"
#include "zoneNode.h"

using namespace qReal::robots::generators::semantics;

SimpleNode::SimpleNode(Id const &idBinded, QObject *parent)
	: NonZoneNode(idBinded, parent)
{
}

void SimpleNode::appendSibling(SemanticNode *node)
{
	parentZone()->appendChild(node);
}

QString SimpleNode::toString() const
{
	return mId.element() + " (s)";
}

QLinkedList<SemanticNode *> SimpleNode::children() const
{
	return QLinkedList<SemanticNode *>();
}
