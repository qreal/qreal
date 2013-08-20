#include "rootNode.h"
#include "simpleNode.h"

using namespace qReal::robots::generators::semantics;

RootNode::RootNode(Id const &initialBlock, QObject *parent)
	: SemanticNode(Id(), parent)
	, mZone(new ZoneNode(this))
{
	mZone->appendChild(new SimpleNode(initialBlock, mZone));
}

void RootNode::debugPrint(int indent)
{
	qDebug() << QString('\t', indent) << "begin";
	mZone->debugPrint(indent + 1);
}
