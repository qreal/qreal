/* Copyright 2015-2016 Kogutich Denis
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

#include "stackFrame.h"

using namespace qReal;
using namespace interpretation;

StackFrame::StackFrame()
	: mBlock(nullptr)
{
}

StackFrame::StackFrame(BlockInterface *block)
	: mBlock(block)
{
}

StackFrame::StackFrame(BlockInterface *block, const QList<QPair<QString, QVariant>> &properties)
	: mBlock(block)
	, mProperties(properties)
{
}

BlockInterface *StackFrame::block() const
{
	return mBlock;
}

const QList<QPair<QString, QVariant>> &StackFrame::properties() const
{
	return mProperties;
}
