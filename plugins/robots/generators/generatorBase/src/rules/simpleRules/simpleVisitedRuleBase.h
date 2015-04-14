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

#include "simpleBlockRuleBase.h"

namespace generatorBase {
namespace semantics {

/// A base for all rules applied to blocks with simple semantics who lead to a
/// visited block.
class SimpleVisitedRuleBase : public SimpleBlockRuleBase
{
public:
	SimpleVisitedRuleBase(SemanticTree *tree, const qReal::Id &id, const LinkInfo &next);

	virtual bool apply();

protected:
	NonZoneNode *mThisNode;  // Doesn`t take ownership
	NonZoneNode *mNextNode;  // Doesn`t take ownership
};

}
}
