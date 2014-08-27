#include "generatorBase/semanticTree/semanticTree.h"

#include "generatorBase/semanticTree/simpleNode.h"
#include "generatorBase/semanticTree/ifNode.h"
#include "generatorBase/semanticTree/loopNode.h"

using namespace generatorBase::semantics;
using namespace qReal;

SemanticTree::SemanticTree(GeneratorCustomizer &customizer, Id const &initialBlock
		, bool isMainTree, QObject *parent)
	: QObject(parent)
	, mCustomizer(customizer)
	, mIsMainTree(isMainTree)
	, mRoot(new RootNode(initialBlock, this))
{
}

Id SemanticTree::initialBlock() const
{
	return mRoot->initialBlock();
}

QList<SemanticTree *> SemanticTree::threads() const
{
	return mSubtrees.values();
}

QString SemanticTree::toString(int indent) const
{
	return mRoot->toString(mCustomizer, indent);
}

SemanticTree *SemanticTree::produceSubtree(Id const &initialNode)
{
	qDebug() << "producing subtree for" << initialNode;
	if (mSubtrees.contains(initialNode)) {
		return mSubtrees[initialNode];
	}

	SemanticTree * const subtree = new SemanticTree(mCustomizer, initialNode, false, this);
	mSubtrees[initialNode] = subtree;
	return subtree;
}

SemanticNode *SemanticTree::produceNodeFor(Id const &id)
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
	case enums::semantics::forkBlock:
		return produceFork(id);
	default:
		/// @todo: Implement switch semantics
		return produceSimple(id);
	}
}

SimpleNode *SemanticTree::produceSimple(Id const &id)
{
	return new SimpleNode(id, this);
}

IfNode *SemanticTree::produceConditional(Id const &id)
{
	return new IfNode(id, this);
}

LoopNode *SemanticTree::produceLoop(Id const &id)
{
	return new LoopNode(id, this);
}

ForkNode *SemanticTree::produceFork(Id const &id)
{
	return new ForkNode(id, this);
}

FinalNode *SemanticTree::produceFinal(Id const &id)
{
	return new FinalNode(id, mIsMainTree, this);
}

NonZoneNode *SemanticTree::findNodeFor(Id const &id)
{
	// Due to inner rule node for given id must exist when we visit it.
	// Also only non-zone nodes can be binded to id.
	// So result MUST be correct. Always.
	return dynamic_cast<NonZoneNode *>(mRoot->findNodeFor(id));
}
