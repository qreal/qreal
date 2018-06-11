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

#include "structurizatorNodeWithBreaks.h"

using namespace generatorBase;

StructurizatorNodeWithBreaks::StructurizatorNodeWithBreaks(IntermediateStructurizatorNode *condition
		, QList<IntermediateStructurizatorNode *> &exitBranches
		, QObject *parent)
	: IntermediateStructurizatorNode(parent)
	, mCondition(condition)
	, mExitBranches(exitBranches)
{
}

IntermediateStructurizatorNode *StructurizatorNodeWithBreaks::condition() const
{
	return mCondition;
}

QList<IntermediateStructurizatorNode *> StructurizatorNodeWithBreaks::exitBranches() const
{
	return mExitBranches;
}

QList<IntermediateStructurizatorNode *> StructurizatorNodeWithBreaks::restBranches() const
{
	return mRestBranches;
}

void StructurizatorNodeWithBreaks::setRestBranches(const QList<IntermediateStructurizatorNode *> &restBranches)
{
	mRestBranches = restBranches;
}

bool StructurizatorNodeWithBreaks::analyzeBreak()
{
	mHasBreakInside = true;
	mBreakWasAnalyzed = true;
	return mHasBreakInside;
}

IntermediateStructurizatorNode::Type StructurizatorNodeWithBreaks::type() const
{
	return nodeWithBreaks;
}

qReal::Id StructurizatorNodeWithBreaks::firstId() const
{
	return mCondition->firstId();
}
