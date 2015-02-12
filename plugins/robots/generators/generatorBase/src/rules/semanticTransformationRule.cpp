#include "semanticTransformationRule.h"

using namespace generatorBase::semantics;
using namespace qReal;

SemanticTransformationRule::SemanticTransformationRule(SemanticTree *tree, const Id &id)
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

bool SemanticTransformationRule::alreadyCreated(const qReal::Id &id)
{
	return mTree->findNodeFor(id) != nullptr;
}

bool SemanticTransformationRule::alreadyCreated(const SemanticTransformationRule::LinkInfo &link)
{
	return alreadyCreated(link.target);
}
