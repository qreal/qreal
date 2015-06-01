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

#pragma once

#include <src/rules/semanticTransformationRule.h>

namespace generatorBase {
namespace semantics {

/// Creates a join of threads in semantic tree. When called from one of joined threads it will finalize execution of
/// that thread. When called from the main thread the execution will proceed after the join is complete.
class JoinRule : public SemanticTransformationRule
{
public:
	JoinRule(SemanticTree *tree
			, const qReal::Id &id
			, const QString &threadId
			, parts::Threads &threadsStorage
			, bool fromMain);

	bool apply() override;

private:
	QString mThreadId;
	parts::Threads &mThreadsStorage;
	bool mFromMain;
};

}
}
