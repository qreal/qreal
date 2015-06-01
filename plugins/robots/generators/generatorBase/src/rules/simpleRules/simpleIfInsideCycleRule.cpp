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

#include "simpleIfInsideCycleRule.h"

using namespace generatorBase::semantics;
using namespace qReal;

SimpleIfInsideCycleRule::SimpleIfInsideCycleRule(SemanticTree *tree
		, const Id &id, const LinkInfo &next)
	: SimpleVisitedRuleBase(tree, id, next)
{
}

bool SimpleIfInsideCycleRule::apply()
{
	if (!SimpleVisitedRuleBase::apply()) {
		return false;
	}

	IfNode * const parentIf = dynamic_cast<IfNode *>(mThisNode->parentZone()->parentNode());
	if (!parentIf || parentIf->parentZone() != mNextNode->parentZone()) {
		return false;
	}

	LoopNode * const parentLoop = makeLoopStartingFrom(mNextNode);

	ZoneNode * const thisZone = mThisNode->parentZone();
	ZoneNode * const otherZone = parentIf->thenZone() == thisZone
			? parentIf->elseZone() : parentIf->thenZone();

	QLinkedList<SemanticNode *> const detachedThisBranch = thisZone->removeStartingFrom(nullptr);
	QLinkedList<SemanticNode *> const detachedOtherBranch = otherZone->removeStartingFrom(nullptr);

	parentIf->appendSiblings(detachedThisBranch);
	parentLoop->appendSiblings(detachedOtherBranch);

	if (thisZone == parentIf->thenZone()) {
		parentIf->invertCondition();
	}

	SimpleNode * const breakNode = mTree->produceSimple(Id());
	breakNode->bindToSyntheticConstruction(SimpleNode::breakNode);
	parentIf->thenZone()->appendChild(breakNode);

	return true;
}
