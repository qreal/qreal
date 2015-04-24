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

#include "mergedSwitchBranchesRule.h"

using namespace generatorBase::semantics;
using namespace qReal;

/// @todo: Unify this rule with if?

MergedSwitchBranchesRule::MergedSwitchBranchesRule(SemanticTree *tree
		, const Id &id, const LinkInfo &next)
	: SimpleVisitedRuleBase(tree, id, next)
{
}

bool MergedSwitchBranchesRule::apply()
{
	if (!SimpleVisitedRuleBase::apply()) {
		return false;
	}

	ZoneNode * const thisZone = mThisNode->parentZone();
	ZoneNode * const nextZone = mNextNode->parentZone();

	SwitchNode * const switchParent = dynamic_cast<SwitchNode *>(thisZone->parentNode());
	SwitchNode * const nextSwitchParent = dynamic_cast<SwitchNode *>(nextZone->parentNode());

	if (switchParent == nextSwitchParent && switchParent && !switchParent->branchesMerged() && thisZone != nextZone) {
		// Then we got merged branches case, handling it...
		QLinkedList<SemanticNode *> const detachedChildren = nextZone->removeStartingFrom(mNextNode);
		switchParent->appendSiblings(detachedChildren);
		switchParent->setBranchesMergedFlag();
		return true;
	}

	// If we then have next branch merging at the same point we can still consider this rule applied.
	return switchParent && switchParent->parentZone() == nextZone && nextZone->nextChild(switchParent) == mNextNode;
}
