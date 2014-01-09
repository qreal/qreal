#include "semanticTree.h"
#include "simpleNode.h"
#include "ifNode.h"
#include "loopNode.h"

using namespace qReal::robots::generators::semantics;

SemanticTree::SemanticTree(GeneratorCustomizer &customizer, Id const &initialBlock
		, bool isMainTree, QObject *parent)
	: QObject(parent)
	, mCustomizer(customizer)
	, mIsMainTree(isMainTree)
	, mRoot(new RootNode(initialBlock, this))
{
}

QString SemanticTree::toString(int indent) const
{
	return mRoot->toString(mCustomizer, indent);
}

SemanticNode *SemanticTree::produceNodeFor(qReal::Id const &id)
{
	switch (mCustomizer.semanticsOf(id)) {
	case enums::semantics::regularBlock:
		return produceSimple(id);
	case enums::semantics::conditionalBlock:
		return produceConditional(id);
	case enums::semantics::loopBlock:
		return produceLoop(id);
	case enums::semantics::finalBlock:
		return produceFinal(id);
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

FinalNode *SemanticTree::produceFinal(qReal::Id const &id)
{
	return new FinalNode(id, mIsMainTree, this);
}

NonZoneNode *SemanticTree::findNodeFor(qReal::Id const &id)
{
	// Due to inner rule node for given id must exist when we visit it.
	// Also only non-zone nodes can be binded to id.
	// So result MUST be correct. Always.
	return static_cast<NonZoneNode *>(mRoot->findNodeFor(id));
}
