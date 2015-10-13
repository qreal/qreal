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

#include "semanticTransformationRule.h"

using namespace generatorBase::semantics;
using namespace qReal;

SemanticTransformationRule::SemanticTransformationRule(SemanticTree *tree, const Id &id)
	: mTree(tree)
	, mId(id)
{
}

SemanticTransformationRule::~SemanticTransformationRule()
{
}

LoopNode *SemanticTransformationRule::makeLoopStartingFrom(NonZoneNode *node)
{
	LoopNode * const endlessLoop = mTree->produceLoop();
	ZoneNode * const parent = node->parentZone();

	QLinkedList<SemanticNode *> const detachedChildren = parent->removeStartingFrom(node);
	endlessLoop->appendChildren(detachedChildren);

	parent->appendChild(endlessLoop);

	return endlessLoop;
}

bool SemanticTransformationRule::alreadyCreated(const qReal::Id &id)
{
	return mTree->findNodeFor(id) != nullptr;
}

bool SemanticTransformationRule::alreadyCreated(const SemanticTransformationRule::LinkInfo &link)
{
	return alreadyCreated(link.target);
}
