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

#pragma once

#include "blockInterface.h"

namespace qReal {
namespace interpretation {

/// An entry in call stack.
class StackFrame
{
public:
	StackFrame();
	explicit StackFrame(BlockInterface *block);
	StackFrame(BlockInterface *block, const QList<QPair<QString, QVariant>> &properties);

	/// Returns an instance of block executing during this frame.
	BlockInterface *block() const;

	/// Returns a list of arguments that were passed to subprogram when this block was called.
	const QList<QPair<QString, QVariant>> &properties() const;

private:
	BlockInterface *mBlock;  // Doesn't have ownership
	/// Maps property name to property value.
	QList<QPair<QString, QVariant>> mProperties;
};

}
}
