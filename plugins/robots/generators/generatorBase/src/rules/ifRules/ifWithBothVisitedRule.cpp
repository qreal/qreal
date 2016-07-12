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

#include "ifWithBothVisitedRule.h"

using namespace generatorBase::semantics;
using namespace qReal;

IfWithBothVisitedRule::IfWithBothVisitedRule(SemanticTree *tree, const Id &id
		, const LinkInfo &thenLink, const LinkInfo &elseLink)
	: IfBlockRuleBase(tree, id, thenLink, elseLink)
{
}

bool IfWithBothVisitedRule::apply()
{
	if (!(alreadyCreated(mThenLink) && alreadyCreated(mElseLink)) || mThenLink.target != mElseLink.target) {
		return false;
	}

	IfNode * const thisNode = static_cast<IfNode *>(mTree->findNodeFor(mId));

	if (thisNode->elseZone()->id() != thisNode->thenZone()->id()) {
		return false;
	}

	thisNode->transformToSimple();

	NonZoneNode * const nextNode = mTree->findNodeFor(mThenLink.target);

	makeLoopStartingFrom(nextNode);

	return true;


}
