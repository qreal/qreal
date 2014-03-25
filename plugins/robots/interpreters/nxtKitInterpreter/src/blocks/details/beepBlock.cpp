#include "beepBlock.h"

#include <utils/tracer.h>
#include <utils/abstractTimer.h>

using namespace nxtKitInterpreter::blocks::details;

BeepBlock::BeepBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: SpeakerBlock(robotModel)
{
}

BeepBlock::~BeepBlock()
{
}

int BeepBlock::duration()
{
	return 500;
}

void BeepBlock::doJob(nxtKitInterpreter::robotModel::parts::NxtSpeaker &speaker)
{
	speaker.beep(duration());
}
