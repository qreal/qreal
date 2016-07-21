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

#include "interpreterCore/interpreter/details/blocksTable.h"

#include <qrkernel/exception/exception.h>

using namespace qReal;
using namespace kitBase::blocksBase;
using namespace interpreterCore::interpreter::details;

BlocksTable::BlocksTable(BlocksFactoryManagerInterface &blocksFactoryManager
		, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager)
	: mBlocksFactoryManager(blocksFactoryManager)
	, mRobotModelManager(robotModelManager)
{
}

interpretation::BlockInterface *BlocksTable::produceBlock(const Id &element)
{
	return mBlocksFactoryManager.block(element, mRobotModelManager.model());
}
