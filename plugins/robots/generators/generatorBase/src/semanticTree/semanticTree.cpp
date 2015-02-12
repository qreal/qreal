#include "generatorBase/semanticTree/semanticTree.h"

#include "generatorBase/semanticTree/simpleNode.h"
#include "generatorBase/semanticTree/ifNode.h"
#include "generatorBase/semanticTree/loopNode.h"

using namespace generatorBase::semantics;
using namespace qReal;

SemanticTree::SemanticTree(GeneratorCustomizer &customizer, const Id &initialBlock
		, bool isMainTree, QObject *parent)
	: QObject(parent)
	, mCustomizer(customizer)
	, mIsMainTree(isMainTree)
	, mRoot(new RootNode(produceNodeFor(initialBlock), this))
{
}

Id SemanticTree::initialBlock() const
{
	return mRoot->initialBlock();
}

QString SemanticTree::toString(int indent, const QString &indentString) const
{
	return mRoot->toString(mCustomizer, indent, indentString);
}

SemanticNode *SemanticTree::produceNodeFor(const Id &id)
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
	case enums::semantics::switchBlock:
		return produceSwitch(id);
	default:
		return nullptr;
	}
}

SimpleNode *SemanticTree::produceSimple(const Id &id)
{
	return new SimpleNode(id, this);
}

IfNode *SemanticTree::produceConditional(const Id &id)
{
	return new IfNode(id, this);
}

LoopNode *SemanticTree::produceLoop(const Id &id)
{
	return new LoopNode(id, this);
}

ForkNode *SemanticTree::produceFork(const Id &id)
{
	return new ForkNode(id, this);
}

SwitchNode *SemanticTree::produceSwitch(const Id &id)
{
	return new SwitchNode(id, this);
}

FinalNode *SemanticTree::produceFinal(const Id &id)
{
	return new FinalNode(id, mIsMainTree, this);
}

NonZoneNode *SemanticTree::findNodeFor(const Id &id)
{
	// Due to inner rule node for given id must exist when we visit it.
	// Also only non-zone nodes can be binded to id.
	// So result MUST be correct. Always.
	return dynamic_cast<NonZoneNode *>(mRoot->findNodeFor(id));
}
