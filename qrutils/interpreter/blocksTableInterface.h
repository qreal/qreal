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

#pragma once

#include <qrutils/interpreter/blockInterface.h>

namespace qReal {
namespace interpretation {

/// Centralized blocks storage. Provides mapping from block ids to objects that implement logic of block.
/// @todo Seems to be unneeded here.
class BlocksTableInterface
{
public:
	virtual ~BlocksTableInterface() {}

	/// Returns object that implement logic of block by given id.
	virtual BlockInterface *block(const qReal::Id &element) = 0;

	/// Clears blocks table.
	virtual void clear() = 0;

	/// Sets "failure" state for all blocks, which allows to abort program execution: if block is failed, it will not
	/// invoke next block.
	virtual void setFailure() = 0;
};

}
}
