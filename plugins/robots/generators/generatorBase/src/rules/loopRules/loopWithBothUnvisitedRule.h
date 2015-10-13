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

#include "loopBlockRuleBase.h"

namespace generatorBase {
namespace semantics {

/// This rule is applied to blocks with loop semantics with both branches unvisited.
/// Simply prepares loop semantic node with two regular branches.
class LoopWithBothUnvisitedRule : public LoopBlockRuleBase
{
public:
	LoopWithBothUnvisitedRule(SemanticTree *tree, const qReal::Id &id
			, const LinkInfo &iterationLink, const LinkInfo &nextLink);

	virtual bool apply();
};

}
}
