#include "beepBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

BeepBlock::BeepBlock(robotParts::Brick &brick, details::AbstractTimer &timer)
	: mBrick(brick), mTimer(timer)
{
	mTimer.setParent(this);
}

void BeepBlock::run()
{
	int const duration = 500;
	mBrick.beep(duration);
	if (!boolProperty("WaitForCompletion"))
		emit done(mNextBlock);
	else {
		connect(&mTimer, SIGNAL(timeout()), this, SLOT(timeout()));
		mTimer.start(duration);
	}
}

void BeepBlock::timeout()
{
	emit done(mNextBlock);
}
