#include "simpleNode.h"

using namespace qReal::robots::generators::semantics;

SimpleNode::SimpleNode(Id const &idBinded, QObject *parent)
{
}

void SimpleNode::debugPrint(int indent)
{
	qDebug() << QString('\t', indent) << mId.element() << "(s)";
}
