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
	const int frequencyValue = eval<int>("Frequency");
	const int durationValue = duration();
	const int volumeValue = eval<int>("Volume");
	if (!errorsOccured()) {
		speaker.playTone(volumeValue, frequencyValue, durationValue);
	}
}
