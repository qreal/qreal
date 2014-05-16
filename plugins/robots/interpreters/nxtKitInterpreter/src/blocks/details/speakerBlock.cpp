#include "speakerBlock.h"

#include <utils/tracer.h>
#include <utils/timelineInterface.h>
#include <utils/abstractTimer.h>
#include <interpreterBase/robotModel/robotModelUtils.h>

using namespace nxtKitInterpreter::blocks::details;
using namespace interpreterBase::robotModel;

SpeakerBlock::SpeakerBlock(RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
	, mTimer(robotModel.timeline().produceTimer())
{
	mTimer->setParent(this);
	connect(mTimer, &utils::AbstractTimer::timeout, this, &SpeakerBlock::timeout);
}

SpeakerBlock::~SpeakerBlock()
{
}

void SpeakerBlock::run()
{
	QString const port = "SpeakerPort";
	robotModel::parts::NxtSpeaker * const speaker
			= RobotModelUtils::findDevice<robotModel::parts::NxtSpeaker>(mRobotModel, port);
	if (speaker) {
		doJob(*speaker);
	} else {
		error(tr("Speaker is not configured (WTF?)"));
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
