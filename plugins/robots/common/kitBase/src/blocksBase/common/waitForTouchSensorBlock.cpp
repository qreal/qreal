#include "kitBase/blocksBase/common/waitForTouchSensorBlock.h"

#include "kitBase/robotModel/robotParts/touchSensor.h"

using namespace kitBase;
using namespace blocksBase::common;
using namespace robotModel;

WaitForTouchSensorBlock::WaitForTouchSensorBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
{
}

void WaitForTouchSensorBlock::responseSlot(int reading)
{
	if (reading == 1) {
		stop();
	}
}

kitBase::robotModel::DeviceInfo WaitForTouchSensorBlock::device() const
{
	return DeviceInfo::create<robotParts::TouchSensor>();
}
