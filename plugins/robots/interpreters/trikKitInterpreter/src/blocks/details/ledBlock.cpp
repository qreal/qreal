#include "ledBlock.h"

#include <QtGui/QColor>

using namespace trikKitInterpreter;
using namespace blocks;
using namespace details;
using namespace interpreterBase::robotModel;

LedBlock::LedBlock(RobotModelInterface &robotModel)
	: interpreterBase::blocksBase::common::DeviceBlock<robotModel::parts::TrikLed>(robotModel)
{
}

void LedBlock::doJob(robotModel::parts::TrikLed &led)
{
	led.setColor(stringProperty("Color"));
	emit done(mNextBlockId);
}
