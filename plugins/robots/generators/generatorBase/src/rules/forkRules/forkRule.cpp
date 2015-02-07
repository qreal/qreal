#include "forkRule.h"

#include "generatorBase/parts/threads.h"

using namespace generatorBase::semantics;
using namespace qReal;

ForkRule::ForkRule(SemanticTree *tree, const Id &id, const QList<LinkInfo> &threads, parts::Threads &threadsStorage)
	: SemanticTransformationRule(tree, id)
	, mThreads(threads)
	, mThreadsStorage(threadsStorage)
{
}

bool ForkRule::apply()
{
	ForkNode * const fork = static_cast<ForkNode *>(mTree->findNodeFor(mId));
	for (const LinkInfo &thread : mThreads) {
		mThreadsStorage.registerThread(thread.target);
		fork->appendThread(thread.target);
	}

	return true;
}
