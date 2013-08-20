#include "zoneNode.h"

using namespace qReal::robots::generators::semantics;

ZoneNode::ZoneNode(QObject *parent)
{
}

void ZoneNode::debugPrint(int indent)
{
	foreach (SemanticNode *node, mChildren) {
		node->debugPrint(indent);
	}
}
