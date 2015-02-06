#include "timerBlock.h"

#include <utils/tracer.h>
#include <utils/timelineInterface.h>
#include <utils/abstractTimer.h>

using namespace interpreterCore::coreBlocks::details;

TimerBlock::TimerBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: mTimer(robotModel.timeline().produceTimer())
{
	mTimer->setParent(this);
	connect(mTimer, &utils::AbstractTimer::timeout, this, &TimerBlock::timeout);
}

TimerBlock::~TimerBlock()
{
}

void TimerBlock::run()
{
	const int interval = eval<int>("Delay");
	if (!errorsOccured()) {
		utils::Tracer::debug(utils::Tracer::blocks, "TimerBlock::run", "interval=" + QString(interval));

		mTimer->start(interval);
	}
}

void TimerBlock::timeout()
{
	utils::Tracer::debug(utils::Tracer::blocks, "TimerBlock::timeout", "emit done(mNextBlock)");
	emit done(mNextBlockId);
}
