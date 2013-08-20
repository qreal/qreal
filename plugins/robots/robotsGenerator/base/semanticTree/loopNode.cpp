#include "loopNode.h"

using namespace qReal::robots::generators::semantics;

LoopNode::LoopNode(Id const &idBinded, QObject *parent)
	: SemanticNode(idBinded, parent)
{
}

void LoopNode::debugPrint(int indent)
{
	qDebug() << QString('\t', indent) << "loop";
	mBodyZone->debugPrint(indent + 1);
}
