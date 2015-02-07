#include "simpleVisitedRuleBase.h"

using namespace generatorBase::semantics;
using namespace qReal;

SimpleVisitedRuleBase::SimpleVisitedRuleBase(SemanticTree *tree, const Id &id
		, const LinkInfo &next)
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
