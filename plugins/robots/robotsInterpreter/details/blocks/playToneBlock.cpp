#include "playToneBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

PlayToneBlock::PlayToneBlock(robotParts::Brick &brick, details::AbstractTimer &timer)
	: mBrick(brick), mTimer(timer)
{
	mTimer.setParent(this);
}

void PlayToneBlock::run()
{
	int const frequency = evaluate("Frequency").toInt();
	int const duration = evaluate("Duration").toInt();
	mBrick.playTone(frequency, duration);
	if (!boolProperty("WaitForCompletion"))
		emit done(mNextBlock);
	else {
		connect(&mTimer, SIGNAL(timeout()), this, SLOT(timeout()));
		mTimer.start(duration);
	}
}

void PlayToneBlock::timeout()
{
	emit done(mNextBlock);
}
