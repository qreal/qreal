#include "timerBlock.h"

#include <utils/tracer.h>
#include <utils/abstractTimer.h>

using namespace interpreterCore::coreBlocks::details;

TimerBlock::TimerBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: mTimer(robotModel.produceTimer())
{
	mTimer->setParent(this);
	connect(mTimer, &utils::AbstractTimer::timeout, this, &TimerBlock::timeout);
}

TimerBlock::~TimerBlock()
{
}

void TimerBlock::run()
{
	int const interval = evaluate("Delay").toInt();
	utils::Tracer::debug(utils::Tracer::blocks, "TimerBlock::run", "interval=" + QString(interval));

	mTimer->start(interval);
}

void TimerBlock::timeout()
{
	utils::Tracer::debug(utils::Tracer::blocks, "TimerBlock::timeout", "emit done(mNextBlock)");
	emit done(mNextBlockId);
}
