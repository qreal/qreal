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

#include "simpleStructurizatorNode.h"

using namespace generatorBase;

SimpleStructurizatorNode::SimpleStructurizatorNode(const qReal::Id &id, QObject *parent)
	: IntermediateStructurizatorNode(parent)
	, mId(id)
{
}

IntermediateStructurizatorNode::Type SimpleStructurizatorNode::type() const
{
	return Type::simple;
}

qReal::Id SimpleStructurizatorNode::firstId() const
{
	return mId;
}

bool SimpleStructurizatorNode::analyzeBreak()
{
	mHasBreakInside = false;
	return mHasBreakInside;
}

qReal::Id SimpleStructurizatorNode::id() const
{
	return mId;
}
