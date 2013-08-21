#include "semanticTree.h"
#include "simpleNode.h"
#include "ifNode.h"
#include "loopNode.h"

using namespace qReal::robots::generators::semantics;

SemanticTree::SemanticTree(GeneratorCustomizer const &customizer, Id const &initialBlock
		, QObject *parent)
	: QObject(parent)
	, mCustomizer(customizer)
	, mRoot(new RootNode(initialBlock, this))
{
}

void SemanticTree::debugPrint()
{
	qDebug() << "Semantic tree:";
	qDebug() << "-----------------";
	mRoot->debugPrint(0);
	qDebug() << "-----------------";
}

SemanticNode *SemanticTree::produceNodeFor(qReal::Id const &id)
{
	switch (mCustomizer.semanticsOf(id)) {
	case enums::semantics::regularBlock:
		return new SimpleNode(id, this);
	case enums::semantics::condidionalBlock:
		return new IfNode(id, this);
	case enums::semantics::loopBlock:
		return new LoopNode(id, this);
	default:
		// TODO: inplement fork and switch
		return new SimpleNode(id, this);
	}
}

SemanticNode *SemanticTree::findNodeFor(qReal::Id const &id)
{
	// Due to inner rule node for given id must exist when we visit it
	// so result MUST NOT be null. Never.
	return mRoot->findNodeFor(id);
}
