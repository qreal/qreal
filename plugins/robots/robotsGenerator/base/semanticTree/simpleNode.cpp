#include "simpleNode.h"
#include "zoneNode.h"

using namespace qReal::robots::generators::semantics;

SimpleNode::SimpleNode(Id const &idBinded, QObject *parent)
	: SemanticNode(idBinded, parent)
{
}

void SimpleNode::appendSibling(SemanticNode *node)
{
	static_cast<ZoneNode *>(mParentNode)->appendChild(node);
}

QString SimpleNode::toString() const
{
	return mId.element() + " (s)";
}

QLinkedList<SemanticNode *> SimpleNode::children() const
{
	return QLinkedList<SemanticNode *>();
}
