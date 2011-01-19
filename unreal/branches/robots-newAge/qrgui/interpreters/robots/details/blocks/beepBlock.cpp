#include "beepBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

BeepBlock::BeepBlock(Id const &graphicalId
		, models::GraphicalModelAssistApi const &graphicalModelApi
		, BlocksTable &blocksTable
		, robotParts::Brick &brick)
	: Block(graphicalId, graphicalModelApi, blocksTable)
	, mBrick(brick)
{
}

void BeepBlock::run()
{
	mBrick.beep(500);
	emit done(mNextBlock);
}
