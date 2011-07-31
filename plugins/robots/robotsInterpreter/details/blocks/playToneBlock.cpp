#include "playToneBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

PlayToneBlock::PlayToneBlock(robotParts::Brick &brick)
	: mBrick(brick)
{
}

void PlayToneBlock::run()
{
	mBrick.playTone(evaluate("Frequency").toInt(), evaluate("Duration").toInt());
	if (!boolProperty("WaitForCompletion"))
		emit done(mNextBlock);
	else {
		mTimer.setInterval(evaluate("Frequency").toInt());
		mTimer.setSingleShot(true);
		connect(&mTimer, SIGNAL(timeout()), this, SLOT(timeout()));
		mTimer.start();
	}
}

void PlayToneBlock::timeout()
{
	emit done(mNextBlock);
}
