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

#include "src/rules/simpleRules/simpleVisitedRuleBase.h"

namespace generatorBase {
namespace semantics {

/// This rule is applied to blocks with simple semantics when it leads to other
/// branch of the parent switch statement ('merged' branches case). Attaches tail
/// of the branch to switch`s siblings. Also handles case when link leads to the
/// switch block`s immediate sibling (when two branches were already merged),
/// terminating current branch in that case.
class MergedSwitchBranchesRule : public SimpleVisitedRuleBase
{
public:
	MergedSwitchBranchesRule(SemanticTree *tree, const qReal::Id &id, const LinkInfo &next);

	bool apply() override;
};

}
}
