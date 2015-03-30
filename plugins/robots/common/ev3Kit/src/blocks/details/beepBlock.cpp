#include "beepBlock.h"

#include <utils/tracer.h>
#include <utils/abstractTimer.h>

using namespace ev3::blocks::details;

BeepBlock::BeepBlock(kitBase::robotModel::RobotModelInterface &robotModel)
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

void BeepBlock::doJob(ev3::robotModel::parts::Ev3Speaker &speaker)
{
	speaker.beep(duration());
}
