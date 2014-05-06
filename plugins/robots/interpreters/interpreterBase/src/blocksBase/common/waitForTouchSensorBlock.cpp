#include "interpreterBase/blocksBase/common/waitForTouchSensorBlock.h"

#include "interpreterBase/robotModel/robotParts/touchSensor.h"

using namespace interpreterBase;
using namespace blocksBase::common;
using namespace robotModel;

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
