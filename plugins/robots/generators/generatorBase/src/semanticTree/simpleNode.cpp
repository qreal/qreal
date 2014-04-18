#include "simpleNode.h"
#include "zoneNode.h"
#include <qrutils/stringUtils.h>

using namespace qReal::robots::generators::semantics;

SimpleNode::SimpleNode(Id const &idBinded, QObject *parent)
	: NonZoneNode(idBinded, parent)
	, mSyntheticBinding(noSytheticBinding)
{
}

QString SimpleNode::toStringImpl(GeneratorCustomizer &customizer, int indent) const
{
	switch (mSyntheticBinding) {
	case breakNode:
		return utils::StringUtils::addIndent(customizer.factory()->breakGenerator(mId
				, customizer)->generate(), indent);
	case continueNode:
		return utils::StringUtils::addIndent(customizer.factory()->continueGenerator(mId
				, customizer)->generate(), indent);
	case gotoNode:
		return utils::StringUtils::addIndent(customizer.factory()->gotoSimpleGenerator(mId
				, customizer)->generate(), indent);
	default:
		return utils::StringUtils::addIndent(customizer.factory()->simpleGenerator(mId
				, customizer)->generate(), indent);
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
