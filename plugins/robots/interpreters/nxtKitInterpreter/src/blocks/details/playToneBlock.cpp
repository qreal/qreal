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
	return evaluate("Frequency").toInt();
}

int PlayToneBlock::duration()
{
	return evaluate("Duration").toInt();
}

void PlayToneBlock::doJob(nxtKitInterpreter::robotModel::parts::NxtSpeaker &speaker)
{
	speaker.playTone(frequency(), duration());
}
