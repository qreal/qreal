#include "timerBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

void TimerBlock::run()
{
	int const interval = intProperty("Delay");

	mTimer.setInterval(interval);
	mTimer.setSingleShot(true);
	connect(&mTimer, SIGNAL(timeout()), this, SLOT(timeout()));
	mTimer.start();
}

void TimerBlock::timeout()
{
	emit done(mNextBlock);
}
