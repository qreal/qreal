#include "simpleNode.h"
#include "zoneNode.h"

using namespace qReal::robots::generators::semantics;

SimpleNode::SimpleNode(Id const &idBinded, QObject *parent)
	: NonZoneNode(idBinded, parent)
	, mSyntheticBinding(noSytheticBinding)
{
}

QString SimpleNode::toString(GeneratorCustomizer &customizer) const
{
	switch (mSyntheticBinding) {
	case breakNode:
		return customizer.factory()->breakGenerator(mId, customizer)->generate();
	case continueNode:
		return customizer.factory()->continueGenerator(mId, customizer)->generate();
	default:
		return customizer.factory()->generatorFor(mId, customizer)->generate();
	}
}

void SimpleNode::bindToSyntheticConstruction(SimpleNode::SyntheticBlockType type)
{
	mSyntheticBinding = type;
}

QLinkedList<SemanticNode *> SimpleNode::children() const
{
	return QLinkedList<SemanticNode *>();
}
