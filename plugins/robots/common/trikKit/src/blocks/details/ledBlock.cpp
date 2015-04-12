#include "ledBlock.h"

using namespace trik;
using namespace blocks;
using namespace details;
using namespace kitBase::robotModel;

LedBlock::LedBlock(RobotModelInterface &robotModel)
	: kitBase::blocksBase::common::DeviceBlock<robotModel::parts::TrikLed>(robotModel)
{
}

void LedBlock::doJob(robotModel::parts::TrikLed &led)
{
	led.setColor(stringProperty("Color"));
	emit done(mNextBlockId);
}
