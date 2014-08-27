#include "generatorBase/semanticTree/forkNode.h"

#include "generatorBase/semanticTree/semanticTree.h"

using namespace generatorBase::semantics;

ForkNode::ForkNode(qReal::Id const &idBinded, QObject *parent)
	: NonZoneNode(idBinded, parent)
{
}

void ForkNode::appendThread(SemanticTree *thread)
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
	QString result = "FORK HERE! CHILDREN:\n";
	for (SemanticTree const *thread : mThreads) {
		result += "==========================" + thread->initialBlock().toString() + "\n";
		result += thread->toString(1);
		result += "\n==========================\n";
	}

	return result;
}
