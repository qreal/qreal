#include "generatorBase/semanticTree/forkNode.h"

#include "generatorBase/semanticTree/semanticTree.h"

using namespace generatorBase::semantics;

ForkNode::ForkNode(qReal::Id const &idBinded, QObject *parent)
	: NonZoneNode(idBinded, parent)
{
}

void ForkNode::appendThread(qReal::Id const &thread)
{
	mThreads << thread;
}

QLinkedList<SemanticNode *> ForkNode::children() const
{
	return {};
}

QString ForkNode::toStringImpl(generatorBase::GeneratorCustomizer &customizer, int indent) const
{
	/// @todo:
	QString result = "FORK HERE!!!\n";

	return result;
}
