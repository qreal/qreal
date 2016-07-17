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

#include "loopWithNextVisitedRule.h"

#include "generatorBase/semanticTree/ifNode.h"

using namespace generatorBase::semantics;
using namespace qReal;

LoopWithNextVisitedRule::LoopWithNextVisitedRule(SemanticTree *tree
		, const Id &id, const LinkInfo &iterationLink, const LinkInfo &nextLink)
	: LoopBlockRuleBase(tree, id, iterationLink, nextLink)
{
}

bool LoopWithNextVisitedRule::apply()
{
	if (alreadyCreated(mIterationLink) || !alreadyCreated(mNextLink)) {
		return false;
	}

	LoopNode * const thisNode = static_cast<LoopNode *>(mTree->findNodeFor(mId));
	NonZoneNode * const nextNode = mTree->findNodeFor(mNextLink.target);

	const bool isLinkToParentLoop = thisNode->parentZone()->parentNode() == nextNode
			&& dynamic_cast<LoopNode *>(nextNode);
	const bool isLinkToMergedBranch = thisNode->parentZone() && nextNode->parentZone()
			&& thisNode->parentZone()->parentNode() == nextNode->parentZone()->parentNode()
			&& dynamic_cast<IfNode *>(thisNode->parentZone()->parentNode());

	if (thisNode->parentZone() == nextNode->parentZone() || isLinkToParentLoop) {
		makeLoopStartingFrom(nextNode);
	} else if (isLinkToMergedBranch) {
		IfNode *ifParent = dynamic_cast<IfNode *>(nextNode->parentZone()->parentNode());
		const QLinkedList<SemanticNode *> detachedChildren = nextNode->parentZone()->removeStartingFrom(nextNode);
		ifParent->appendSiblings(detachedChildren);
	}

	SemanticNode * const iterationNode = mTree->produceNodeFor(mIterationLink.target);
	thisNode->bodyZone()->appendChild(iterationNode);

	return true;
}
