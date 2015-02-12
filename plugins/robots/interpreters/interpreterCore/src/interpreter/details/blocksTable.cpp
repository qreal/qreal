#include "interpreterCore/interpreter/details/blocksTable.h"

#include <qrkernel/exception/exception.h>

using namespace qReal;
using namespace interpreterBase::blocksBase;
using namespace interpreterCore::interpreter::details;

BlocksTable::BlocksTable(BlocksFactoryManagerInterface &blocksFactoryManager
		, const interpreterBase::robotModel::RobotModelManagerInterface &robotModelManager)
	: mBlocksFactoryManager(blocksFactoryManager)
	, mRobotModelManager(robotModelManager)
{
}

interpretation::BlockInterface *BlocksTable::produceBlock(const Id &element)
{
	return mBlocksFactoryManager.block(element, mRobotModelManager.model());
}
