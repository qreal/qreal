#include "beepBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

BeepBlock::BeepBlock(robotParts::Brick &brick, details::AbstractTimer *timer)
	: mBrick(brick), mTimer(timer)
{
	mTimer->setParent(this);
}

void BeepBlock::run()
{
	mBrick.beep(500);
	if (!boolProperty("WaitForCompletion"))
		emit done(mNextBlock);
	else {
		connect(mTimer, SIGNAL(timeout()), this, SLOT(timeout()));
		mTimer->start(500);
	}
}

void BeepBlock::timeout()
{
	emit done(mNextBlock);
}