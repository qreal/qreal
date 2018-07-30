/* Copyright 2018 Konstantin Batoev
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

#include "selfLoopStructurizatorNode.h"

using namespace generatorBase;

SelfLoopStructurizatorNode::SelfLoopStructurizatorNode(IntermediateStructurizatorNode *bodyNode, QObject *parent)
	: IntermediateStructurizatorNode(parent)
	, mBodyNode(bodyNode)
{
}

IntermediateStructurizatorNode *SelfLoopStructurizatorNode::bodyNode() const
{
	return mBodyNode;
}

bool SelfLoopStructurizatorNode::analyzeBreak()
{
	if (mBreakWasAnalyzed) {
		return mHasBreakInside;
	}

	mHasBreakInside = mBodyNode->analyzeBreak();
	mBreakWasAnalyzed = true;
	return mHasBreakInside;
}

IntermediateStructurizatorNode::Type SelfLoopStructurizatorNode::type() const
{
	return Type::infiniteloop;
}

qReal::Id SelfLoopStructurizatorNode::firstId() const
{
	return mBodyNode->firstId();
}
