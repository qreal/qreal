#include "joinRule.h"

#include "generatorBase/parts/threads.h"

using namespace generatorBase::semantics;

JoinRule::JoinRule(SemanticTree *tree
		, const qReal::Id &id
		, const QString &threadId
		, parts::Threads &threadsStorage
		, bool fromMain)
	: SemanticTransformationRule(tree, id)
	, mThreadId(threadId)
	, mThreadsStorage(threadsStorage)
	, mFromMain(fromMain)
{

}

bool JoinRule::apply()
{
	JoinNode *join = static_cast<JoinNode *>(mTree->findNodeFor(mId));
	join->setThreadId(mThreadId);

	if (!mFromMain) {
		mThreadsStorage.addJoin(mId, mThreadId);
	}

	return true;
}
