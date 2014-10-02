#include "playToneBlock.h"

#include <utils/tracer.h>
#include <utils/abstractTimer.h>

using namespace ev3KitInterpreter::blocks::details;

PlayToneBlock::PlayToneBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: SpeakerBlock(robotModel)
{
}

int PlayToneBlock::duration()
{
	return eval<int>("Duration");
}

void PlayToneBlock::doJob(ev3KitInterpreter::robotModel::parts::Ev3Speaker &speaker)
{
	int const frequencyValue = eval<int>("Frequency");
	int const durationValue = duration();
	int const volumeValue = eval<int>("Volume");
	if (!errorsOccured()) {
		speaker.playTone(volumeValue, frequencyValue, durationValue);
	}
}
