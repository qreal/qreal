#include "semanticTransformationRule.h"

using namespace qReal::robots::generators::semantics;

SemanticTransformationRule::SemanticTransformationRule(SemanticTree *tree, Id const &id)
	: mTree(tree)
	, mId(id)
{
}

SemanticTransformationRule::~SemanticTransformationRule()
{
}

LoopNode *SemanticTransformationRule::makeLoopStartingFrom(NonZoneNode *node)
{
	LoopNode * const endlessLoop = mTree->produceLoop();
	ZoneNode * const parent = node->parentZone();

	QLinkedList<SemanticNode *> const detachedChildren = parent->removeStartingFrom(node);
	endlessLoop->appendChildren(detachedChildren);

	parent->appendChild(endlessLoop);

	return endlessLoop;
}
