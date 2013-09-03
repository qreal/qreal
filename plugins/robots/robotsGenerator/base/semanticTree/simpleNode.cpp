#include "simpleNode.h"
#include "zoneNode.h"

using namespace qReal::robots::generators::semantics;

SimpleNode::SimpleNode(Id const &idBinded, QObject *parent)
	: NonZoneNode(idBinded, parent)
	, mSyntheticBinding(noSytheticBinding)
{
}

QString SimpleNode::toString(GeneratorCustomizer &customizer, int indent) const
{
	switch (mSyntheticBinding) {
	case breakNode:
		return addIndent(customizer.factory()->breakGenerator(mId, customizer)->generate(), indent);
	case continueNode:
		return addIndent(customizer.factory()->continueGenerator(mId, customizer)->generate(), indent);
	default:
		return addIndent(customizer.factory()->simpleGenerator(mId, customizer)->generate(), indent);
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
