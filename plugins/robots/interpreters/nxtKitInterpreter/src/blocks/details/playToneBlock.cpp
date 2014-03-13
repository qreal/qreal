#include "playToneBlock.h"

#include <utils/tracer.h>
#include <utils/abstractTimer.h>

using namespace nxtKitInterpreter::blocks::details;

PlayToneBlock::PlayToneBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: SpeakerBlock(robotModel)
{
}

PlayToneBlock::~PlayToneBlock()
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

void PlayToneBlock::doJob(nxtKitInterpreter::robotModel::real::parts::Speaker &speaker)
{
	speaker.playTone(frequency(), duration());
}
