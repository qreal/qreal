#include "generatorBase/semanticTree/rootNode.h"

#include "generatorBase/semanticTree/simpleNode.h"

using namespace generatorBase::semantics;
using namespace qReal;

RootNode::RootNode(SemanticNode * const initialBlock, QObject *parent)
	: SemanticNode(Id(), parent)
	, mZone(new ZoneNode(this))
	, mInitialBlock(initialBlock->id())
{
	mZone->setParentNode(this);
	mZone->appendChild(initialBlock);
}

Id RootNode::initialBlock() const
{
	return mInitialBlock;
}

QString RootNode::toStringImpl(GeneratorCustomizer &customizer, int indent, QString const &indentString) const
{
	return mZone->toString(customizer, indent, indentString);
}

QLinkedList<SemanticNode *> RootNode::children() const
{
	return QLinkedList<SemanticNode *>() << mZone;
}
