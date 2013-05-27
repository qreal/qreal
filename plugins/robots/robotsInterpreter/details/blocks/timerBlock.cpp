#include "timerBlock.h"

#include "../tracer.h"

using namespace qReal;
using namespace interpreters::robots::details;
using namespace blocks;

TimerBlock::TimerBlock(AbstractTimer * const timer)
	: mTimer(timer)
{
	mTimer->setParent(this);
	connect(mTimer, SIGNAL(timeout()), this, SLOT(timeout()));
}

void TimerBlock::run()
{
	int const interval = evaluate("Delay").toInt();
	Tracer::debug(tracer::blocks, "TimerBlock::run", "interval=" + QString(interval));

	mTimer->start(interval);
}

void TimerBlock::timeout()
{
	Tracer::debug(tracer::blocks, "TimerBlock::timeout", "emit done(mNextBlock)");
	emit done(mNextBlock);
}