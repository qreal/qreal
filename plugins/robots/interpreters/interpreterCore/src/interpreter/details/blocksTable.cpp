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
