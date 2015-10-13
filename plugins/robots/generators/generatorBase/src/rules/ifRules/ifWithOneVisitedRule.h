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

#include "ifBlockRuleBase.h"

namespace generatorBase {
namespace semantics {

/// This rule is applied to blocks with if semantics with exactly one branch visited.
/// If branch comes into the node in the same with current if block zone creates
/// cycle with conditional break, otherwise this rule is considered non-applicable.
class IfWithOneVisitedRule : public IfBlockRuleBase
{
public:
	IfWithOneVisitedRule(SemanticTree *tree, const qReal::Id &id
			, const LinkInfo &thenLink, const LinkInfo &elseLink);

	virtual bool apply();
};

}
}
