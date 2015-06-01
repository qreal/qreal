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

#include "simpleVisitedOneZoneRule.h"

#include "generatorBase/semanticTree/loopNode.h"

using namespace generatorBase::semantics;
using namespace qReal;

SimpleVisitedOneZoneRule::SimpleVisitedOneZoneRule(SemanticTree *tree, const Id &id
		, const LinkInfo &next)
	: SimpleVisitedRuleBase(tree, id, next)
{
}

bool SimpleVisitedOneZoneRule::apply()
{
	if (!SimpleVisitedRuleBase::apply()) {
		return false;
	}

	if (mNextNode->parentZone() != mThisNode->parentZone()) {
		// If we have backward edge to loop - it`s ok, simply ignoring it
		return mThisNode->parentZone()->parentNode() == mNextNode
				&& dynamic_cast<LoopNode *>(mNextNode);
	}

	makeLoopStartingFrom(mNextNode);

	return true;
}
