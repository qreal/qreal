#include "initialBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

InitialBlock::InitialBlock(Id const &graphicalId
		, models::GraphicalModelAssistApi const &graphicalModelApi
		, BlocksTable &blocksTable
		, RobotModel &robotModel)
	: Block(graphicalId, graphicalModelApi, blocksTable)
	, mRobotModel(robotModel)
{
}

void InitialBlock::run()
{
	mRobotModel.init();
	emit done(mNextBlock);
}
