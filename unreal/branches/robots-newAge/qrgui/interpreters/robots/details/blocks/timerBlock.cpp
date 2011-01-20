#include "timerBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

TimerBlock::TimerBlock(Id const &graphicalId
		, models::GraphicalModelAssistApi const &graphicalModelApi
		, models::LogicalModelAssistApi const &logicalModelApi
		, BlocksTable &blocksTable)
	: Block(graphicalId, graphicalModelApi, blocksTable)
	, mGraphicalModelApi(graphicalModelApi)
	, mLogicalModelApi(logicalModelApi)
{
}

void TimerBlock::run()
{
	Id const logicalId = mGraphicalModelApi.logicalId(id());
	int const interval = mLogicalModelApi.propertyByRoleName(logicalId, "Delay").toInt();

	mTimer.setInterval(interval);
	mTimer.setSingleShot(true);
	connect(&mTimer, SIGNAL(timeout()), this, SLOT(timeout()));
	mTimer.start();
}

void TimerBlock::timeout()
{
	emit done(mNextBlock);
}
