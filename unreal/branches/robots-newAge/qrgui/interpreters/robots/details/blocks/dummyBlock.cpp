#include "dummyBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

DummyBlock::DummyBlock(Id const &graphicalId
		, models::GraphicalModelAssistApi const &graphicalModelApi
		, models::LogicalModelAssistApi const &logicalModelApi
		, BlocksTable &blocksTable)
	: Block(graphicalId, graphicalModelApi, blocksTable)
{
}

void DummyBlock::run()
{
	mTimer.setInterval(500);
	mTimer.setSingleShot(true);
	connect(&mTimer, SIGNAL(timeout()), this, SLOT(timeout()));
	mTimer.start();
}

void DummyBlock::timeout()
{
	emit done(mNextBlock);
}
