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

#include <qrutils/interpreter/blocksTableBase.h>
#include <kitBase/robotModel/robotModelManagerInterface.h>

#include "interpreterCore/managers/blocksFactoryManagerInterface.h"

namespace interpreterCore {
namespace interpreter {
namespace details {

/// Implementation of blocks table functionality required by kitBase.
/// Provides mapping from block ids to objects that implement logic of block, also creates blocks when needed, so
/// clients can simply request a block by given id, and a block table will do the rest.
/// Also supports operations that shall be performed on all blocks in a system, such as setting failure or idle flags.
/// and objects implementing logic of that blocks.
class BlocksTable : public qReal::interpretation::BlocksTableBase
{
public:
	/// Constructor.
	/// @param blocksFactoryManager - a factory manager that is used to create new blocks when needed.
	/// @param robotModelManager - has a reference to current robot model.
	BlocksTable(BlocksFactoryManagerInterface &blocksFactoryManager
			, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager);

private:
	qReal::interpretation::BlockInterface *produceBlock(const qReal::Id &element) override;

	BlocksFactoryManagerInterface &mBlocksFactoryManager;
	const kitBase::robotModel::RobotModelManagerInterface &mRobotModelManager;
};

}
}
}
