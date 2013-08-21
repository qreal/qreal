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
		return produceSimple(id);
	case enums::semantics::condidionalBlock:
		return produceConditional(id);
	case enums::semantics::loopBlock:
		return produceLoop(id);
	default:
		// TODO: implement fork and switch
		return produceSimple(id);
	}
}

SimpleNode *SemanticTree::produceSimple(qReal::Id const &id)
{
	return new SimpleNode(id, this);
}

IfNode *SemanticTree::produceConditional(qReal::Id const &id)
{
	return new IfNode(id, this);
}

LoopNode *SemanticTree::produceLoop(qReal::Id const &id)
{
	return new LoopNode(id, this);
}

NonZoneNode *SemanticTree::findNodeFor(qReal::Id const &id)
{
	// Due to inner rule node for given id must exist when we visit it.
	// Also only non-zone nodes can be binded to id.
	// So result MUST NOT be null. Never.
	return static_cast<NonZoneNode *>(mRoot->findNodeFor(id));
}
