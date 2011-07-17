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
	if (!boolProperty("WaitForCompletion"))
		emit done(mNextBlock);
	else {
		mTimer.setInterval(500);
		mTimer.setSingleShot(true);
		connect(&mTimer, SIGNAL(timeout()), this, SLOT(timeout()));
		mTimer.start();
	}
}

void BeepBlock::timeout()
{
	emit done(mNextBlock);
}
