#include "generatorBase/semanticTree/nonZoneNode.h"

using namespace generatorBase::semantics;
using namespace qReal;

NonZoneNode::NonZoneNode(Id const &idBinded, QObject *parent)
	: SemanticNode(idBinded, parent)
{
}

ZoneNode *NonZoneNode::parentZone()
{
	return static_cast<ZoneNode *>(mParentNode);
}

void NonZoneNode::appendSibling(SemanticNode *node)
{
	parentZone()->appendChild(node);
}

void NonZoneNode::appendSiblings(QLinkedList<SemanticNode *> const &nodes)
{
	parentZone()->appendChildren(nodes);
}

void NonZoneNode::insertSiblingAfterThis(SemanticNode *node)
{
	parentZone()->insertAfrer(this, node);
}
