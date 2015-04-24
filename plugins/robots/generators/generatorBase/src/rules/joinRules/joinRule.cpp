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
