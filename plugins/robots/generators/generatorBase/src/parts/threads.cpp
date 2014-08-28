#include "generatorBase/parts/threads.h"

using namespace generatorBase;
using namespace parts;

void Threads::registerThread(qReal::Id const &id)
{
	if (!mUnprocessedThreads.contains(id) && !mProcessedThreads.keys().contains(id)) {
		mUnprocessedThreads.insert(id);
	}
}

void Threads::threadProcessed(qReal::Id const &id, semantics::SemanticTree &tree)
{
	if (!mUnprocessedThreads.contains(id)) {
		return;
	}

	mProcessedThreads[id] = &tree;
	mUnprocessedThreads.remove(id);
}

bool Threads::hasUnprocessedThreads() const
{
	return !mUnprocessedThreads.empty();
}

qReal::Id Threads::nextUnprocessedThread() const
{
	return *mUnprocessedThreads.begin();
}

QString Threads::generateCode() const
{
	QString result = "THREADS:\n";
	for (semantics::SemanticTree const *thread : mProcessedThreads.values()) {
		result += "==========================" + thread->initialBlock().toString() + "\n";
		result += thread->toString(1);
		result += "\n==========================\n";
	}

	return result;
}
