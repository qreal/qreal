#pragma once

#include <qrutils/interpreter/blocksTableBase.h>
#include <interpreterBase/robotModel/robotModelManagerInterface.h>

#include "src/managers/blocksFactoryManagerInterface.h"

namespace interpreterCore {
namespace interpreter {
namespace details {

/// Implementation of blocks table functionality required by InterpreterBase.
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
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager);

private:
	qReal::interpretation::BlockInterface *produceBlock(qReal::Id const &element) override;

	BlocksFactoryManagerInterface &mBlocksFactoryManager;
	interpreterBase::robotModel::RobotModelManagerInterface const &mRobotModelManager;
};

}
}
}
