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

#include "simpleUnvisitedRule.h"
#include "generatorBase/semanticTree/simpleNode.h"

using namespace generatorBase::semantics;
using namespace qReal;

SimpleUnvisitedRule::SimpleUnvisitedRule(SemanticTree *tree, const Id &id
		, const LinkInfo &next)
	: SimpleBlockRuleBase(tree, id, next)
{
}

bool SimpleUnvisitedRule::apply()
{
	if (alreadyCreated(mNextLink)) {
		return false;
	}

	SemanticNode * const nextNode = mTree->produceNodeFor(mNextLink.target);
	SimpleNode * const thisNode = static_cast<SimpleNode *>(mTree->findNodeFor(mId));
	thisNode->appendSibling(nextNode);

	return true;
}
