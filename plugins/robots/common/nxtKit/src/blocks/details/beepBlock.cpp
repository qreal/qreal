#include "beepBlock.h"

#include <utils/tracer.h>
#include <utils/abstractTimer.h>

using namespace nxt::blocks::details;

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

void BeepBlock::doJob(robotModel::parts::NxtSpeaker &speaker)
{
	speaker.beep(duration());
}
