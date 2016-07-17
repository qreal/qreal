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

#include "src/rules/semanticTransformationRule.h"

namespace generatorBase {
namespace semantics {

/// A base class for all transformation rules for loops
class LoopBlockRuleBase : public SemanticTransformationRule
{
public:
	LoopBlockRuleBase(SemanticTree *tree, const qReal::Id &id
			, const LinkInfo &iterationLink, const LinkInfo &nextLink);

protected:
	const LinkInfo &mIterationLink;
	const LinkInfo &mNextLink;
};

}
}
