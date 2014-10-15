#include "beepBlock.h"

#include <utils/tracer.h>
#include <utils/abstractTimer.h>

using namespace ev3KitInterpreter::blocks::details;

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

void BeepBlock::doJob(ev3KitInterpreter::robotModel::parts::Ev3Speaker &speaker)
{
	speaker.beep(duration());
}
