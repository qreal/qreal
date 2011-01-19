#include "simpleTestBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

SimpleTestBlock::SimpleTestBlock(Id const &graphicalId
		, models::GraphicalModelAssistApi const &graphicalModelApi
		, BlocksTable &blocksTable)
	: Block(graphicalId, graphicalModelApi, blocksTable)
{
}

void SimpleTestBlock::run()
{
	mTimer.setInterval(500);
	mTimer.setSingleShot(true);
	connect(&mTimer, SIGNAL(timeout()), this, SLOT(timeout()));
	mTimer.start();
}

void SimpleTestBlock::timeout()
{
	emit done(mNextBlock);
}
