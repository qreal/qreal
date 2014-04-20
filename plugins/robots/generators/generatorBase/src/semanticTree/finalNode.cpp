#include "generatorBase/semanticTree/finalNode.h"

#include <qrutils/stringUtils.h>

using namespace generatorBase::semantics;
using namespace qReal;

FinalNode::FinalNode(Id const &idBinded, bool inMainDigram, QObject *parent)
	: NonZoneNode(idBinded, parent)
	, mInMainDiagram(inMainDigram)
{
}

QString FinalNode::toStringImpl(GeneratorCustomizer &customizer, int indent) const
{
	return utils::StringUtils::addIndent(customizer.factory()->finalNodeGenerator(mId
			, customizer, mInMainDiagram)->generate(), indent);
}

QLinkedList<SemanticNode *> FinalNode::children() const
{
	return QLinkedList<SemanticNode *>();
}
