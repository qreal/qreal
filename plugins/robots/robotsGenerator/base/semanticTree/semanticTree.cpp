#include "semanticTree.h"

using namespace qReal::robots::generators::semantics;

SemanticTree::SemanticTree(Id const &initialBlock, QObject *parent)
	: QObject(parent)
	, mRoot(new RootNode(initialBlock, this))
{
}

void SemanticTree::debugPrint()
{
	qDebug() << "Semantic tree:";
	qDebug() << "-----------------";
	int indent = 0;
	mRoot->debugPrint(indent);
	qDebug() << "-----------------";
}
