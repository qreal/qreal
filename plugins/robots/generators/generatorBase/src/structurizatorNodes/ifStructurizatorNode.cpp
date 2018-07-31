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

#include "ifStructurizatorNode.h"

using namespace generatorBase;

IfStructurizatorNode::IfStructurizatorNode(IntermediateStructurizatorNode *condition
		, IntermediateStructurizatorNode *thenBranch
		, IntermediateStructurizatorNode *elseBranch
		, IntermediateStructurizatorNode *exit
		, QObject *parent)
	: IntermediateStructurizatorNode(parent)
	, mCondition(condition)
	, mThenBranch(thenBranch)
	, mElseBranch(elseBranch)
	, mExit(exit)
{
}

IntermediateStructurizatorNode *IfStructurizatorNode::condition() const
{
	return mCondition;
}

IntermediateStructurizatorNode *IfStructurizatorNode::thenBranch() const
{
	return mThenBranch;
}

IntermediateStructurizatorNode *IfStructurizatorNode::elseBranch() const
{
	return mElseBranch;
}

IntermediateStructurizatorNode *IfStructurizatorNode::exit() const
{
	return mExit;
}

bool IfStructurizatorNode::analyzeBreak()
{
	mHasBreakInside = mThenBranch->analyzeBreak();
	if (mElseBranch) {
		mHasBreakInside |= mElseBranch->analyzeBreak();
	}

	return mHasBreakInside;
}

IntermediateStructurizatorNode::Type IfStructurizatorNode::type() const
{
	return Type::ifThenElseCondition;
}

qReal::Id IfStructurizatorNode::firstId() const
{
	return mCondition->firstId();
}
