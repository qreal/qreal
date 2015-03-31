#include "speakerBlock.h"

#include <utils/tracer.h>
#include <utils/timelineInterface.h>
#include <utils/abstractTimer.h>
#include <kitBase/robotModel/robotModelUtils.h>

using namespace ev3::blocks::details;
using namespace kitBase::robotModel;

SpeakerBlock::SpeakerBlock(RobotModelInterface &robotModel)
	: kitBase::blocksBase::common::DeviceBlock<robotModel::parts::Ev3Speaker>(robotModel)
	, mTimer(robotModel.timeline().produceTimer())
{
	mTimer->setParent(this);
	connect(mTimer, &utils::AbstractTimer::timeout, this, &SpeakerBlock::timeout);
}

void SpeakerBlock::run()
{
	DeviceBlock::run();
	if (errorsOccured()) {
		return;
	}

	if (!boolProperty("WaitForCompletion"))
		emit done(mNextBlockId);
	else {
		mTimer->start(duration());
	}
}

void SpeakerBlock::timeout()
{
	utils::Tracer::debug(utils::Tracer::blocks, "TimerBlock::timeout", "emit done(mNextBlock)");
	emit done(mNextBlockId);
}
