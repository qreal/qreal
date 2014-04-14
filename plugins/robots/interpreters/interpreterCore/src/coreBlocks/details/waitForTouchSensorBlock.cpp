#include "waitForTouchSensorBlock.h"

#include <interpreterBase/robotModel/robotParts/touchSensor.h>

using namespace interpreterCore::coreBlocks::details;
using namespace interpreterBase::robotModel;

WaitForTouchSensorBlock::WaitForTouchSensorBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
{
}

WaitForTouchSensorBlock::~WaitForTouchSensorBlock()
{
}

void WaitForTouchSensorBlock::responseSlot(int reading)
{
	if (reading == 1) {
		stop();
	}
}

interpreterBase::robotModel::DeviceInfo WaitForTouchSensorBlock::device() const
{
	return DeviceInfo::create<robotParts::TouchSensor>();
}
