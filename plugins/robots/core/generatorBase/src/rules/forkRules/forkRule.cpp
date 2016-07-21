/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
