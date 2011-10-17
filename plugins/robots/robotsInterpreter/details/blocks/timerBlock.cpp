#include "timerBlock.h"

#include "../tracer.h"

using namespace qReal;
using namespace interpreters::robots::details;
using namespace blocks;

void TimerBlock::run()
{
	int const interval = evaluate("Delay").toInt();
	Tracer::debug(tracer::blocks, "TimerBlock::run", "interval=" + QString(interval));

	mTimer.setInterval(interval);
	mTimer.setSingleShot(true);
	connect(&mTimer, SIGNAL(timeout()), this, SLOT(timeout()), Qt::UniqueConnection);
	mTimer.start();
}

void TimerBlock::timeout()
{
	Tracer::debug(tracer::blocks, "TimerBlock::timeout", "emit done(mNextBlock)");
	emit done(mNextBlock);
}
