#include "forkRule.h"

#include "generatorBase/parts/threads.h"

using namespace generatorBase::semantics;
using namespace qReal;

ForkRule::ForkRule(SemanticTree *tree, const Id &id, const QList<LinkInfo> &threads
		, const QHash<qReal::Id, QString> &threadIds, parts::Threads &threadsStorage)
	: SemanticTransformationRule(tree, id)
	, mThreads(threads)
	, mThreadIds(threadIds)
	, mThreadsStorage(threadsStorage)
{
}

bool ForkRule::apply()
{
	ForkNode * const fork = static_cast<ForkNode *>(mTree->findNodeFor(mId));
	for (const LinkInfo &thread : mThreads) {
		mThreadsStorage.registerThread(thread.target, mThreadIds[thread.linkId]);
		fork->appendThread(thread.target, mThreadIds[thread.linkId]);
	}

	return true;
}
