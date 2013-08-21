#include "zoneNode.h"

using namespace qReal::robots::generators::semantics;

ZoneNode::ZoneNode(QObject *parent)
	: SemanticNode(Id(), parent)
{
}

void ZoneNode::appendChild(SemanticNode *node)
{
	mChildren << node;
	node->setParentNode(this);
}

void ZoneNode::removeChild(SemanticNode *node)
{
	mChildren.removeOne(node);
	node->setParentNode(NULL);
}

SemanticNode *ZoneNode::removeLast()
{
	SemanticNode *result = mChildren.last();
	mChildren.removeLast();
	result->setParentNode(NULL);
	return result;
}

QString ZoneNode::toString() const
{
	return "";
}

QLinkedList<SemanticNode *> ZoneNode::children() const
{
	return mChildren;
}
