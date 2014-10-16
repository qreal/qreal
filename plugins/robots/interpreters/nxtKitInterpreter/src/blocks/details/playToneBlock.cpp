#include "playToneBlock.h"

#include <utils/tracer.h>
#include <utils/abstractTimer.h>

using namespace nxtKitInterpreter::blocks::details;

PlayToneBlock::PlayToneBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: SpeakerBlock(robotModel)
{
}

int PlayToneBlock::duration()
{
	return eval<int>("Duration");
}

void PlayToneBlock::doJob(nxtKitInterpreter::robotModel::parts::NxtSpeaker &speaker)
{
	int const frequencyValue = eval<int>("Frequency");
	int const durationValue = duration();
	if (!errorsOccured()) {
		speaker.playTone(frequencyValue, durationValue);
	}
}
