#include "playToneBlock.h"

#include <utils/tracer.h>
#include <utils/abstractTimer.h>

using namespace nxtKitInterpreter::blocks::details;

PlayToneBlock::PlayToneBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: SpeakerBlock(robotModel)
{
}

int PlayToneBlock::frequency()
{
	return eval<int>("Frequency");
}

int PlayToneBlock::duration()
{
	return eval<int>("Duration");
}

void PlayToneBlock::doJob(nxtKitInterpreter::robotModel::parts::NxtSpeaker &speaker)
{
	int const frequencyValue = frequency();
	int const durationValue = duration();
	if (!wereParserErrors()) {
		speaker.playTone(frequencyValue, durationValue);
	}
}
