#include "beepBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

BeepBlock::BeepBlock(robotParts::Brick &brick)
	: mBrick(brick)
{
}

void BeepBlock::run()
{
	mBrick.beep(500);
	emit done(mNextBlock);
}
