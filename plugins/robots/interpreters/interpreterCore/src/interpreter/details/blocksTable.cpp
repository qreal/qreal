#include "blocksTable.h"

#include <qrkernel/exception/exception.h>

using namespace qReal;
using namespace interpreterBase::blocksBase;
using namespace interpreterCore::interpreter::details;

BlocksTable::BlocksTable(BlocksFactoryManagerInterface &blocksFactoryManager
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager)
	: mBlocksFactoryManager(blocksFactoryManager)
	, mRobotModelManager(robotModelManager)
{
}

interpretation::BlockInterface *BlocksTable::produceBlock(Id const &element)
{
	return mBlocksFactoryManager.block(element, mRobotModelManager.model());
}
