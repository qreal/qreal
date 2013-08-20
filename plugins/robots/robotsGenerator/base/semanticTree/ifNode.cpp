#include "ifNode.h"

using namespace qReal::robots::generators::semantics;

IfNode::IfNode(Id const &idBinded, QObject *parent)
	: SemanticNode(idBinded, parent)
{
}

void IfNode::debugPrint(int indent)
{
	qDebug() << QString('\t', indent) << "if";
	qDebug() << QString('\t', indent + 1) << "then";
	mThenZone->debugPrint(indent + 2);
	qDebug() << QString('\t', indent + 1) << "else";
	mElseZone->debugPrint(indent + 2);
}
