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

#include "blockStructurizatorNode.h"

using namespace generatorBase;

BlockStructurizatorNode::BlockStructurizatorNode(IntermediateStructurizatorNode *firstNode
		, IntermediateStructurizatorNode *secondNode
		, QObject *parent)
	: IntermediateStructurizatorNode(parent)
	, mFirstNode(firstNode)
	, mSecondNode(secondNode)
{
}

IntermediateStructurizatorNode *BlockStructurizatorNode::firstNode() const
{
	return mFirstNode;
}

IntermediateStructurizatorNode *BlockStructurizatorNode::secondNode() const
{
	return mSecondNode;
}

bool BlockStructurizatorNode::analyzeBreak()
{
	if (mBreakWasAnalyzed) {
		return mHasBreakInside;
	}

	mHasBreakInside = mFirstNode->analyzeBreak() || mSecondNode->analyzeBreak();
	mBreakWasAnalyzed = true;
	return mHasBreakInside;
}

IntermediateStructurizatorNode::Type BlockStructurizatorNode::type() const
{
	return Type::block;
}

qReal::Id BlockStructurizatorNode::firstId() const
{
	return firstNode()->firstId();
}
