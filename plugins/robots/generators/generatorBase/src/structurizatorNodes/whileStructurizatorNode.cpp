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

#include "whileStructurizatorNode.h"

using namespace generatorBase;

WhileStructurizatorNode::WhileStructurizatorNode(IntermediateStructurizatorNode *headNode
		, IntermediateStructurizatorNode *bodyNode
		, IntermediateStructurizatorNode *exitNode
		, QObject *parent)
	: IntermediateStructurizatorNode(parent)
	, mHeadNode(headNode)
	, mBodyNode(bodyNode)
	, mExitNode(exitNode)
{
}

IntermediateStructurizatorNode *WhileStructurizatorNode::headNode() const
{
	return mHeadNode;
}

IntermediateStructurizatorNode *WhileStructurizatorNode::bodyNode() const
{
	return mBodyNode;
}

IntermediateStructurizatorNode *WhileStructurizatorNode::exitNode() const
{
	return mExitNode;
}

bool WhileStructurizatorNode::analyzeBreak()
{
	if (mBreakWasAnalyzed) {
		return mHasBreakInside;
	}

	mHasBreakInside = mHeadNode->analyzeBreak() || mBodyNode->analyzeBreak();
	mBreakWasAnalyzed = true;
	return mHasBreakInside;
}

IntermediateStructurizatorNode::Type WhileStructurizatorNode::type() const
{
	return Type::whileloop;
}

qReal::Id WhileStructurizatorNode::firstId() const
{
	return mHeadNode->firstId();
}
