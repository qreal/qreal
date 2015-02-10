#include "generatorBase/semanticTree/simpleNode.h"

#include <qrutils/stringUtils.h>
#include "generatorBase/semanticTree/zoneNode.h"

using namespace generatorBase::semantics;
using namespace qReal;

SimpleNode::SimpleNode(const Id &idBinded, QObject *parent)
	: NonZoneNode(idBinded, parent)
	, mSyntheticBinding(noSytheticBinding)
{
}

QString SimpleNode::toStringImpl(GeneratorCustomizer &customizer, int indent, const QString &indentString) const
{
	switch (mSyntheticBinding) {
	case breakNode:
		return utils::StringUtils::addIndent(customizer.factory()->breakGenerator(mId
				, customizer)->generate(), indent, indentString);
	case continueNode:
		return utils::StringUtils::addIndent(customizer.factory()->continueGenerator(mId
				, customizer)->generate(), indent, indentString);
	case gotoNode:
		return utils::StringUtils::addIndent(customizer.factory()->gotoSimpleGenerator(mId
				, customizer)->generate(), indent, indentString);
	default:
		return utils::StringUtils::addIndent(customizer.factory()->simpleGenerator(mId
				, customizer)->generate(), indent, indentString);
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
