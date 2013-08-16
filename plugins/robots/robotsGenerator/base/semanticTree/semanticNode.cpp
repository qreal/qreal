#include "semanticNode.h"

using namespace qReal::robots::generators::semantics;

SemanticNode::SemanticNode(Id const &idBinded, QObject *parent)
	: QObject(parent)
	, mId(idBinded)
{
}

void SemanticNode::bindTo(qReal::Id const &id)
{
	mId = id;
}
