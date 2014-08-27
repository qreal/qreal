#include "forkRule.h"

using namespace generatorBase::semantics;
using namespace qReal;

ForkRule::ForkRule(SemanticTree *tree, Id const &id, QList<LinkInfo> const &threads)
	: SemanticTransformationRule(tree, id)
	, mThreads(threads)
{
}

bool ForkRule::apply()
{
	ForkNode * const fork = static_cast<ForkNode *>(mTree->findNodeFor(mId));
	for (LinkInfo const &thread : mThreads) {
		fork->appendThread(mTree->produceSubtree(thread.target));
	}

	return true;
}
