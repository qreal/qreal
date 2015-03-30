#include "playToneBlock.h"

#include <utils/tracer.h>
#include <utils/abstractTimer.h>

using namespace nxt::blocks::details;

PlayToneBlock::PlayToneBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: SpeakerBlock(robotModel)
{
}

int PlayToneBlock::duration()
{
	return eval<int>("Duration");
}

void PlayToneBlock::doJob(robotModel::parts::NxtSpeaker &speaker)
{
	const int frequencyValue = eval<int>("Frequency");
	const int durationValue = duration();
	if (!errorsOccured()) {
		speaker.playTone(frequencyValue, durationValue);
	}
}
