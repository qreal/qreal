#include "simpleVisitedRuleBase.h"

using namespace qReal::robots::generators::semantics;

SimpleVisitedRuleBase::SimpleVisitedRuleBase(SemanticTree *tree, Id const &id
		, LinkInfo const &next)
	: SimpleBlockRuleBase(tree, id, next)
{
}

bool SimpleVisitedRuleBase::apply()
{
	if (!alreadyCreated(mNextLink)) {
		return false;
	}

	mThisNode = mTree->findNodeFor(mId);
	mNextNode = mTree->findNodeFor(mNextLink.target);

	return true;
}
