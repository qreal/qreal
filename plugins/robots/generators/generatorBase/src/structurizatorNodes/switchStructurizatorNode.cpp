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

#include "switchStructurizatorNode.h"

using namespace generatorBase;

SwitchStructurizatorNode::SwitchStructurizatorNode(IntermediateStructurizatorNode *condition
		, const QList<IntermediateStructurizatorNode *> &branches
		, IntermediateStructurizatorNode *exit
		, QObject *parent)
	: IntermediateStructurizatorNode(parent)
	, mCondition(condition)
	, mBranches(QList<IntermediateStructurizatorNode *>(branches))
	, mExit(exit)
{
}

IntermediateStructurizatorNode *SwitchStructurizatorNode::condition() const
{
	return mCondition;
}

QList<IntermediateStructurizatorNode *> SwitchStructurizatorNode::branches() const
{
	return mBranches;
}

IntermediateStructurizatorNode *SwitchStructurizatorNode::exit() const
{
	return mExit;
}

bool SwitchStructurizatorNode::analyzeBreak()
{
	if (mBreakWasAnalyzed) {
		return mHasBreakInside;
	}

	mHasBreakInside = false;
	for (IntermediateStructurizatorNode *node : mBranches) {
		mHasBreakInside |= node->analyzeBreak();
	}

	mBreakWasAnalyzed = true;
	return mHasBreakInside;
}

IntermediateStructurizatorNode::Type SwitchStructurizatorNode::type() const
{
	return Type::switchCondition;
}

qReal::Id SwitchStructurizatorNode::firstId() const
{
	return mCondition->firstId();
}
